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
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "LiarMaxPlugin.h"

#define LiarMaxPlugin_CLASS_ID	Class_ID(0x8035d5d8, 0xdb84fbae)


class LiarMaxPluginClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new LiarMaxPlugin(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return LiarMaxPlugin_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("LiarMaxPlugin"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetLiarMaxPluginDesc() { 
	static LiarMaxPluginClassDesc LiarMaxPluginDesc;
	return &LiarMaxPluginDesc; 
}





INT_PTR CALLBACK LiarMaxPluginOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static LiarMaxPlugin* imp = nullptr;

	switch(message) {
		case WM_INITDIALOG:
			imp = (LiarMaxPlugin *)lParam;
			imp->GetDialogMgr()->Init(hWnd, lParam);
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;
		case WM_COMMAND:
			imp->GetDialogMgr()->ChangeHandle(hWnd, wParam);
			break;
		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- LiarMaxPlugin -------------------------------------------------------
LiarMaxPlugin::LiarMaxPlugin():liarPluginCfg(new Liar::LiarPluginCfg()), m_liarDialogMgr(new Liar::LiarMaxDialogMgr())
{
}

LiarMaxPlugin::~LiarMaxPlugin() 
{

}

int LiarMaxPlugin::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *LiarMaxPlugin::Ext(int /*i*/)
{		
	#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("model");
}

const TCHAR *LiarMaxPlugin::LongDesc()
{
	#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("liar Model & Mesh & Anim file");
}
	
const TCHAR *LiarMaxPlugin::ShortDesc() 
{			
	#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("liar");
}

const TCHAR *LiarMaxPlugin::AuthorName()
{			
	#pragma message(TODO("Return ASCII Author name"))
	return _T("");
}

const TCHAR *LiarMaxPlugin::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR *LiarMaxPlugin::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *LiarMaxPlugin::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int LiarMaxPlugin::Version()
{				
	#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void LiarMaxPlugin::ShowAbout(HWND /*hWnd*/)
{			
	// Optional
}

BOOL LiarMaxPlugin::SupportsOptions(int /*ext*/, DWORD /*options*/)
{
	#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}


int	LiarMaxPlugin::DoExport(const TCHAR* /*name*/, ExpInterface* /*ei*/, Interface* /*ip*/, BOOL suppressPrompts, DWORD /*options*/)
{
	#pragma message(TODO("Implement the actual file Export here and"))

	if(!suppressPrompts)
		DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				LiarMaxPluginOptionsDlgProc, (LPARAM)this);

	#pragma message(TODO("return TRUE If the file is exported properly"))
	return FALSE;
}


