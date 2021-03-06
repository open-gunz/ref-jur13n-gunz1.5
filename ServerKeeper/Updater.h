#pragma once


/*
 Patch process step.
  1. Download patch file from file server.
  2. Prepare patching work.(ex. execute 'do.bat' batch file.)
  3. Find server process and Stop server process.
  4. Temp folder file copy to main server folder.
  5. Restart server process.
 */

#include "afxinet.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

const static char* CONFINGURE_FILE = "./Configure.ini";

class CUpdater
{
public:
	CUpdater(void);
	virtual ~CUpdater(void);

	inline isInit() { return m_bSuccess; }

	static CUpdater& GetInst()
	{
		static CUpdater Updater;
		return Updater;
	}

	const string GetServerPath() const { return m_strGameServerPath; }
	const string GetAgnetPath() const { return m_strAgentServerPath; }
	const CString GetServerRootDir() const { return m_strLocalDestServerRootDir; }
	const int GetUpdaterPort() const { return m_nPort; }

	CString& GetServerIP() { return m_strServerIP; }

	inline void SetFtpConnection( CFtpConnection* pFtpConnection )
	{
		m_pFtpConnection = pFtpConnection;
	}

	void ParseIP( const string& strIPs );
	
	bool Init();
	bool ConnectPatchFileServer();
	void DisconnectPatchFileServer();

	bool IsConnectableIP( const string& strIP );
	bool IsEnableServerPatch() { return m_IsEnableServerPatch; }
	bool IsEnableAgentPatch()  { return m_IsEnableAgentPatch; }
	
	/// MatchServer.
	bool DownloadServer();
	bool StopServerProcess();
	bool FindServerProcess();
	bool PrepareServerPatching();
	bool ServerPatching();
	bool StartServerProcess();
	// bool FindAgentServerProcess();

	/// MatchAgent.
	bool StartAgent();
	bool StopAgent();
	bool DownloadAgent();
	bool FindAgentProcess();
	bool PrepareAgentPatching();
	bool AgentPatching();
		
private :
	bool LoadConfigureFile();

	const DWORD MakeFileCRC32( const string& strFileName );
	
	bool DownloadPatchInfoFile();
	bool DownloadServerPatchFile();
	bool DownloadAgentPatchFile();
	bool CopyServerPatchFile( const char* pszDstName );	
	bool CopyAgentPatchFile( const char* pszDstName );
	BOOL MakeSubDirToServerRealDir( const CString& strSubDirPath );
	BOOL MaekSubDirToAgentRealDir( const CString& strSubDirPath );

	bool			FindProcess( const string strProcess );
	bool			StopProcess( const string strProcessPath );
	bool			StartProcess( const string strProcessPath );
	bool			LoadListFile();
	bool			DownloadPatchFile( const string& strSrcDir );
	void			RemakeTempDirectory( const char* pszDirName );
	bool			FindFolder( const CString& strCur, const CString& strDst );
	const CString	TokenizeFromBack( const CString& strTok, CString& strSrc );
	
	// ?????????? ???? ???? ?????? ????.
	inline const CString MakeTempDirFilePath( const CString& strFileName )
	{
		return m_strLocalTempDir + "\\" + strFileName;
	}
	// ?????????? Patch file?? ?????? ???????? ?????????? ?????? ????.
	inline const CString MakeDownloadedPatchInfoFilePath()
	{
		return m_strLocalTempDir + "\\" + m_strFtpListFile;
	}
	// ?????? MatchServer?????? ???? ?????? ????.
	inline const CString MakeRealServerDirFilePath( const CString& strFileName )
	{
		return m_strLocalDestServerRootDir + "\\" + strFileName;
	}
	/// ?????? MatchAgent?????? ???? ?????? ????.
	inline const CString MakeRealAgentDirFilePath( const CString& strFileName )
	{
		return m_strLocalDestAgentRootDir + "\\" + strFileName; 
	}

	inline bool IsValidDir( const CString& strDir )
	{
		return ( ('.' != strDir) && (".." != strDir) && (-1 == strDir.Find('.')) );
	}

private :
	/// FTP Server????.
	CInternetSession*	m_pInetSession;
	CFtpConnection*		m_pFtpConnection;
	CString				m_strServerName;
	CString				m_strUserName;
	CString				m_strPassword;
	CString				m_strMatchServerDir;
	CString				m_strMatchAgentDir;
	///

	// ???? ????.
	CString				m_strFtpRootDir;			
	CString				m_strFtpListFile;
	CString				m_strLocalTempDir;	// Ftp File Server?????? ???????? ?????? ?????????? ?????? ???????? ????.
	CString				m_strServerIP;		// Keeper?? ?????? ???? IP
	CString				m_strTok;			// ???? ???????????? ???????? ?????? ????.
	///

	/// MatchServer????.
	CString				m_strLocalDestServerRootDir;	// ?????? Server?????? Root???? ????. ???? ?????? ?????? ??.
	string				m_strGameServerPath;			// server???? ?????? ???? ????.
	string				m_strGameServerName;			// server process????.
	DWORD				m_dwServerCRC32;
	bool				m_IsEnableServerPatch;			// Patch?????????? server?????? ?????? true?? ??????.
	///

	/// MatchAgent????.
	CString				m_strLocalDestAgentRootDir;		// ?????? Agent?? Root???? ????. ???? ?????? ?????? ??.
	string				m_strAgentServerPath;			// agent???? ?????? ???? ????.
	string				m_strAgentServerName;			// agent process????.
	DWORD				m_dwAgentCRC32;
	bool				m_IsEnableAgentPatch;			// Patch?????????? agent?????? ?????? true?? ??????.
	///

	bool				m_bSuccess;

	vector< string >	m_vConnectableIP;
	int					m_nPort;
};

#define GetUpdater CUpdater::GetInst()
