#include "LiarMaxPlugin.h"
#include "3dsmaxport.h"
#include "LiarMaxDialogMgr.h"
#include "LiarMaxNodeParse.h"
#include "LiarPluginWrite.h"

namespace Liar
{
	LiarMaxDialogMgr::LiarMaxDialogMgr():m_pluginCfg(new Liar::LiarPluginCfg())
	{
	}


	LiarMaxDialogMgr::~LiarMaxDialogMgr()
	{
		delete m_pluginCfg;
		m_pluginCfg = nullptr;
	}

	void LiarMaxDialogMgr::Init(HWND hWnd, LPARAM lParam)
	{
		DLSetWindowLongPtr(hWnd, lParam);
		CenterWindow(hWnd, GetParent(hWnd));

		ISpinnerControl* spin = GetISpinner(GetDlgItem(hWnd, IDC_STATIC_FRAME_SPIN));

		spin->LinkToEdit(GetDlgItem(hWnd, IDC_STATIC_FRAME), EDITTYPE_INT);
		spin->SetLimits(0, 100, TRUE);
		spin->SetScale(1.0f);
		spin->SetValue(m_pluginCfg->staticFrame, FALSE);
		ReleaseISpinner(spin);

		spin = GetISpinner(GetDlgItem(hWnd, IDC_SAMPLE_FRAME_SPIN));
		spin->LinkToEdit(GetDlgItem(hWnd, IDC_SAMPLE_FRAME), EDITTYPE_INT);
		spin->SetLimits(1, 100, TRUE);
		spin->SetScale(1.0f);
		spin->SetValue(m_pluginCfg->framePerSample, FALSE);
		ReleaseISpinner(spin);

		CheckDlgButton(hWnd, IDC_EXP_GEOMETRY, m_pluginCfg->exportGeom);
		CheckDlgButton(hWnd, IDC_EXP_REVERT_UV, m_pluginCfg->exportGeom);
		CheckDlgButton(hWnd, IDC_EXP_MATERIAL, m_pluginCfg->exportMaterial);
		CheckDlgButton(hWnd, IDC_EXP_MODIFIERS, m_pluginCfg->exportModifier);
		CheckDlgButton(hWnd, IDC_EXP_ANIM, m_pluginCfg->exportAnim);

		//CheckDlgButton(hWnd, IDC_EXP_OBJECT_SPACE, m_pluginCfg->objectSpace);
		//CheckDlgButton(hWnd, IDC_EXP_BASE_ON_SMGRPS, m_pluginCfg->faceBaseOnSmgrsp);
		//CheckDlgButton(hWnd, IDC_EXP_MAPPING_CHANNEL, m_pluginCfg->mappingChannels);
		CheckDlgButton(hWnd, IDC_EXP_VERTEX_NORMAL, m_pluginCfg->vertexNormal);
		CheckDlgButton(hWnd, IDC_EXP_VERTEX_UV, m_pluginCfg->textureCoord);
		CheckDlgButton(hWnd, IDC_EXP_VERTEX_CORLOR, m_pluginCfg->vertexColor);

		/*CheckDlgButton(hWnd, IDC_EXP_RELATIVE, m_pluginCfg->relative);
		CheckDlgButton(hWnd, IDC_EXP_CONSTRAINTS, m_pluginCfg->constraints);
		CheckDlgButton(hWnd, IDC_EXP_SAMPLECONT, m_pluginCfg->forceSample);
		CheckDlgButton(hWnd, IDC_EXP_QUATERNIONS, m_pluginCfg->quaternions);*/

		CheckDlgButton(hWnd, IDC_EXP_SKELETON, m_pluginCfg->skeleton);
		CheckDlgButton(hWnd, IDC_EXP_SKIN, m_pluginCfg->skin);
		//CheckDlgButton(hWnd, IDC_EXP_GENMOD, m_pluginCfg->general);


		int ID = IDC_COORD_MAX + m_pluginCfg->coordSystemType;
		CheckRadioButton(hWnd, IDC_COORD_MAX, IDC_COORD_OPENGL, ID);

		SetGeoEnable(hWnd);
		SetControllerEnable(hWnd);
		SetModifierEnable(hWnd);
		SetVertexNormalEnable(hWnd);

		//Versioning
		TCHAR Title[256];
		float version = static_cast<float>(m_pluginCfg->pluginVersion / 100);
		_stprintf(Title, _T("LiarPlugin version %.3f"), version);
		SetWindowText(hWnd, Title);
	}

	void LiarMaxDialogMgr::ChangeHandle(HWND hWnd, WPARAM wParam, bool selectd, const std::string& path)
	{
		switch (LOWORD(wParam))
		{
		case IDC_EXP_GEOMETRY:
			m_pluginCfg->exportGeom = IsDlgButtonChecked(hWnd, IDC_EXP_GEOMETRY) > 0 ? true : false;
			SetGeoEnable(hWnd);
			break;
		case IDC_EXP_ANIM:
			m_pluginCfg->exportAnim = IsDlgButtonChecked(hWnd, IDC_EXP_ANIM) > 0 ? true : false;
			SetControllerEnable(hWnd);
			break;
		case IDC_EXP_MODIFIERS:
			m_pluginCfg->exportModifier = IsDlgButtonChecked(hWnd, IDC_EXP_MODIFIERS) > 0 ? true : false;
			SetModifierEnable(hWnd);
			break;
		case IDC_EXP_VERTEX_NORMAL:
			m_pluginCfg->vertexNormal = IsDlgButtonChecked(hWnd, IDC_EXP_VERTEX_NORMAL) > 0 ? true : false;
			SetVertexNormalEnable(hWnd);
			break;
		case IDC_BUTTON_OK:
		{
			ConfirmHandle(hWnd);

			Liar::LiarMaxNodeParse* parse = new Liar::LiarMaxNodeParse();
			int result = parse->ParseRootNode(m_pluginCfg, selectd);

			if (result == TRUE)
			{
				Liar::LiarPluginWrite::WriteLiarNode(parse, m_pluginCfg, path);
				MessageBox(hWnd, L"导出成功!", L"导出", MB_ICONINFORMATION);
			}
			else
			{
				MessageBox(hWnd, L"导出失败!", L"导出", MB_ICONINFORMATION);
			}

			delete parse;

			EndDialog(hWnd, 1);
			break;
		}
		case IDCANCEL:
			EndDialog(hWnd, 0);
			break;
		}
	}

	void LiarMaxDialogMgr::ConfirmHandle(HWND hWnd)
	{
		m_pluginCfg->exportGeom = IsDlgButtonChecked(hWnd, IDC_EXP_GEOMETRY) > 0 ? true : false;
		m_pluginCfg->exportGeom = IsDlgButtonChecked(hWnd, IDC_EXP_REVERT_UV) > 0 ? true : false;
		m_pluginCfg->exportMaterial = IsDlgButtonChecked(hWnd, IDC_EXP_MATERIAL) > 0 ? true : false;
		m_pluginCfg->exportModifier = IsDlgButtonChecked(hWnd, IDC_EXP_MODIFIERS) > 0 ? true : false;
		m_pluginCfg->exportAnim = IsDlgButtonChecked(hWnd, IDC_EXP_ANIM) > 0 ? true : false;

		m_pluginCfg->vertexNormal = IsDlgButtonChecked(hWnd, IDC_EXP_VERTEX_NORMAL) > 0 ? true : false;
		m_pluginCfg->textureCoord = IsDlgButtonChecked(hWnd, IDC_EXP_VERTEX_UV) > 0 ? true : false;
		m_pluginCfg->vertexColor = IsDlgButtonChecked(hWnd, IDC_EXP_VERTEX_CORLOR) > 0 ? true : false;

		m_pluginCfg->skeleton = IsDlgButtonChecked(hWnd, IDC_EXP_SKELETON) > 0 ? true : false;
		m_pluginCfg->skin = IsDlgButtonChecked(hWnd, IDC_EXP_SKIN) > 0 ? true : false;

		if (IsDlgButtonChecked(hWnd, IDC_COORD_MAX))
		{
			m_pluginCfg->coordSystemType = IGameConversionManager::CoordSystem::IGAME_MAX;
		}
		else if (IsDlgButtonChecked(hWnd, IDC_COORD_OPENGL))
		{
			m_pluginCfg->coordSystemType = IGameConversionManager::CoordSystem::IGAME_OGL;
		}
		else
		{
			m_pluginCfg->coordSystemType = IGameConversionManager::CoordSystem::IGAME_D3D;
		}

		ISpinnerControl* spin = GetISpinner(GetDlgItem(hWnd, IDC_STATIC_FRAME));
		m_pluginCfg->staticFrame = spin->GetIVal();
		ReleaseISpinner(spin);

		spin = GetISpinner(GetDlgItem(hWnd, IDC_SAMPLE_FRAME));
		m_pluginCfg->framePerSample = spin->GetIVal();
		ReleaseISpinner(spin);

		WStr szName;
		HWND hAnimNameEdit = ::GetDlgItem(hWnd, IDC_EXP_ANIM);
		GetWindowText(hAnimNameEdit, szName);
		Liar::StringUtil::GetWSTR2Char(szName, m_pluginCfg->animName);

		HWND hSkeleNameEdit = ::GetDlgItem(hWnd, IDC_EXP_ANIM);
		GetWindowText(hSkeleNameEdit, szName);
		Liar::StringUtil::GetWSTR2Char(szName, m_pluginCfg->skeletonName);
	}

	void LiarMaxDialogMgr::SetGeoEnable(HWND hWnd)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_VERTEX_NORMAL), m_pluginCfg->exportGeom);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_VERTEX_UV), m_pluginCfg->exportGeom);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_VERTEX_CORLOR), m_pluginCfg->exportGeom);
	}

	void LiarMaxDialogMgr::SetControllerEnable(HWND hWnd)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANIM), m_pluginCfg->exportAnim);
	}

	void LiarMaxDialogMgr::SetModifierEnable(HWND hWnd)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_SKELETON), m_pluginCfg->exportModifier);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_SKIN), m_pluginCfg->exportModifier);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SKELETON), m_pluginCfg->exportModifier);
	}

	void LiarMaxDialogMgr::SetVertexNormalEnable(HWND)
	{
		
	}
}
