#include "MKKBallPlugin.h"
#include "MKKSocket.h"

#if (defined(VDJ_WIN))
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#endif

Network network;
int Network::net_id = 0;

//-----------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnLoad()
{
	setlocale(LC_ALL, "hu_HU.iso88592");
	/// Valami varázslat...
	SetConsoleOutputCP(1251);


	// ADD YOUR CODE HERE WHEN THE PLUGIN IS CALLED
	//Felkészülünk a plugin betöltésére.
	//Naplófájl létrehozása
	char folder[64];
	GetStringInfo("get_vdj_folder", folder, 64);

	std::string workingDir = folder;
	workingDir += "Plugins\\VideoEffect\\MKKLog.txt";

	logger.generateLogFile("C:\\Users\\Márk\\Documents\\VirtualDJ\\Plugins\\VideoEffect\\MKKLog.txt");
	logger.createLog(UPDATE, "OnLoad: A Plugin betöltése megtörtént.");
	
	network.init();

	//Paraméterek inicializálása
	is_connected = false;
	strcpy(connection_status, "Offline");
	strcpy(ip_address, "localhost");
	strcpy(port, "5503");


	//Paraméterek betöltése (gombok)
	DeclareParameterSwitch(&conSwitch_status, SWITCH_CONNECT, "Vetítés", "C", false);
	DeclareParameterString(connection_status, LB_CONN_STATUS, "Kapcsolat", "CS", 30);

	//DeclareParameterString(ip_address, LB_IPADDR, "IP cím", "IP", sizeof(ip_address));
	
	//DeclareParameterButton(&is_connected, BTN_SETPORT, "Port", "P");
	//DeclareParameterButton(&is_connected, BTN_SETMSG, "A DJ üzenete", "M");



	VideoWidth = 0;
	VideoHeight = 0;

#if (defined(VDJ_WIN))

	D3DDevice = NULL;
	D3DTexture = NULL;
	D3DSurface = NULL;

#elif (defined(VDJ_MAC))

	glContext = 0;
	GLTexture = 0;

#endif

	return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->PluginName = "MKK Vetítő rendszer";
	infos->Author = "Mihalovits Márk";
	infos->Description = "VirtualDJ aktuális számairól küld információt";
	infos->Version = "1.5";
	infos->Flags = 0x00; // VDJFLAG_PROCESSLAST if you want to ensure that all other effects are processed first
	infos->Bitmap = NULL;
	

	return S_OK;
}
//---------------------------------------------------------------------------
ULONG VDJ_API MKKBallMaker::Release()
{
	// ADD YOUR CODE HERE WHEN THE PLUGIN IS RELEASED
	logger.createLog(UPDATE, "OnRelease: A Plugin feloldása megtörtént.");

	network.shutdown_server();

	delete this;
	return 0;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnDeviceInit()
{
	// ADD YOUR CODE HERE
	logger.createLog(UPDATE, "OnDeviceInit: A Plugint kezelő ablak felépült.");
	HRESULT hr;

#if (defined(VDJ_WIN))

	hr = GetDevice(VdjVideoEngineDirectX9, (void**)&D3DDevice);
	if (hr != S_OK || D3DDevice == NULL) return S_FALSE;

#elif (defined(VDJ_MAC))

	glContext = CGLGetCurrentContext();

#endif

	// Size of the Video Window
	VideoWidth = width;
	VideoHeight = height;

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnDeviceClose()
{
	// ADD YOUR CODE HERE
	logger.createLog(UPDATE, "OnDeviceClose: A Plugint kezelő ablak bezárult.");
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnStart()
{
	// ADD YOUR CODE HERE

	int logID = TRACK_INFO + rand() % 1000;

	//logger.createLog(UPDATE, "OnStart: A Plugin elindult.", logID);

	MKKTrack nowPlaying = MKKTrack(this);

	//logger.createLog(INFO, "Loaded song!", 1);
	//logger.createLog(INFO, "Artist: " + nowPlaying.getArtist(), logID);
	//logger.createLog(INFO, "Title: " + nowPlaying.getTitle(), logID);
	//logger.createLog(INFO, "Genre: " + nowPlaying.getGenre(), logID);
	//logger.createLog(INFO, "Next Song: " + nowPlaying.getNextGenre(), logID);

	if (is_connected == TRUE) {

		network.send_message(nowPlaying.createJSON());

	}

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnStop()
{
	// ADD YOUR CODE HERE
	logger.createLog(UPDATE, "OnStop: A Plugin leállt.");
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnDraw()
{
	// ADD YOUR CODE HERE
	HRESULT hr;
	TVertex *vertices;

	if (VideoWidth != width || VideoHeight != height)
	{
		hr = OnVideoResize(width, height);
	}

#if (defined(VDJ_WIN))

	hr = GetTexture(VdjVideoEngineDirectX9, (void **)&D3DTexture, &vertices);
	hr = D3DTexture->GetSurfaceLevel(0, &D3DSurface);

	SAFE_RELEASE(D3DTexture);
	SAFE_RELEASE(D3DSurface);

#elif (defined(VDJ_MAC))

	hr = GetTexture(VdjVideoEngineOpenGL, (void **)&GLTexture, &vertices);

	// glEnable(GL_TEXTURE_RECTANGLE_EXT);
	// glBindTexture(GL_TEXTURE_RECTANGLE_EXT, GLTexture);
	// glBegin(GL_TRIANGLE_STRIP);

	//for(int j=0;j<4;j++)
	//{
	//glColorD3D(vertices[j].color);
	//glTexCoord2f(vertices[j].tu, vertices[j].tv);
	//glVertex3f(vertices[j].position.x, vertices[j].position.y, vertices[j].position.z);
	//}

	//glEnd();
#endif


	hr = DrawDeck();
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT MKKBallMaker::OnVideoResize(int VidWidth, int VidHeight)
{
	// OnDeviceClose();
	// OnDeviceInit();
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnParameter(int id) {
	//Gombok működése

	HRESULT hr;

	switch (id) {

	case SWITCH_CONNECT:

		if (is_connected == true) {
			network.shutdown_server();
			network.init();
			is_connected = false;
			strcpy(connection_status,"Offline");
		}
		else 
		{
			switch (network.resolve(ip_address, port)) {

			default:
			case ERR_NO_DOMAIN:
				strcpy(connection_status, "Nincs Cím");
				is_connected = false;
				break;

			case ERR_GETADDRINFO_FAIL:
				strcpy(connection_status, "Rossz Cím");
				is_connected = false;
				break;

			case ERR_NOERR:
				network.connect_server() == ERR_NOERR ? strcpy(connection_status, "Online") : strcpy(connection_status, "Hibás");
				is_connected = true;
				break;
			}
		}

	case BTN_SETADDR:

		break;

	case BTN_SETMSG:

		break;

	case BTN_SETPORT:

		break;

	default:
		break;
	}


	hr = SendCommand("effect_redraw");

	//Refresh strings
	logger.createLog(INFO, "HR = " + hr);

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnGetParameterString(int id, char *outParam, int outParamSize) {
	switch (id)
	{
	default:
	case SWITCH_CONNECT:
		sprintf(outParam, "Yeah!");
		break;
	}

	return S_OK;

}




MKKTrack::MKKTrack(MKKBallMaker *parent)
{
	char* title = new char[max_length];
	char* artist = new char[max_length];
	char* genre = new char[max_length];
	char* next_genre = new char[max_length];

	parent->GetStringInfo("deck active get_loaded_song 'title'", title, max_length);
	parent->GetStringInfo("deck active get_loaded_song 'artist'", artist, max_length);
	parent->GetStringInfo("deck active get_loaded_song 'genre'", genre, max_length);
	parent->GetStringInfo("get_automix_song 'genre'", next_genre, max_length);
	parent->GetInfo("get_time_min 'remain'", &time_rem_min);
	parent->GetInfo("get_time_sec 'remain'", &time_rem_sec);

	s_title = title;
	s_artist = artist;
	s_genre = genre;
	s_next_genre = next_genre;

	free(title);
	free(artist);
	free(genre);
	free(next_genre);

}

std::string MKKTrack::createJSON()
{
	TrackData["title"] = s_title;
	TrackData["artist"] = s_artist;
	TrackData["genre"] = s_genre;
	TrackData["next_song"] = s_next_genre;
	TrackData["min_left"] = time_rem_min;
	TrackData["sec_left"] = time_rem_sec;

	return TrackData.dump();
}
