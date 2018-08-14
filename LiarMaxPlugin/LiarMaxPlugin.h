#pragma once

//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Includes for Plugins
// AUTHOR: 
//***************************************************************************/

#include "3dsmaxsdk_preinclude.h"
#include "resource.h"
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
//SIMPLE TYPE


#include <impexp.h>

#include <LiarPluginCfg.h>
#include "LiarMaxDialogMgr.h"


extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

class LiarMaxPlugin : public SceneExport {
public:
	//Constructor/Destructor
	LiarMaxPlugin();
	~LiarMaxPlugin();

	int				ExtCount();					// Number of extensions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

	BOOL SupportsOptions(int ext, DWORD options);
	int  DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0);

public:
	Liar::LiarPluginCfg* liarPluginCfg;

private:
	Liar::LiarMaxDialogMgr* m_liarDialogMgr;

public:
	Liar::LiarMaxDialogMgr* GetDialogMgr() { return m_liarDialogMgr; };
};
