#include "LiarMaxPlugin.h"
#include "3dsmaxport.h"
#include "LiarMaxDialogMgr.h"

namespace Liar
{
	LiarMaxDialogMgr::LiarMaxDialogMgr():m_pluginCfg(nullptr)
	{
	}


	LiarMaxDialogMgr::~LiarMaxDialogMgr()
	{
		m_pluginCfg = nullptr;
	}

	void LiarMaxDialogMgr::Init(HWND hWnd, LPARAM lParam)
	{
		LiarMaxPlugin* exp = (LiarMaxPlugin *)lParam;
		m_pluginCfg = exp->liarPluginCfg;
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
		CheckDlgButton(hWnd, IDC_EXP_SPLINES, m_pluginCfg->exportSplines);
		CheckDlgButton(hWnd, IDC_EXP_CONTROLLERS, m_pluginCfg->exportController);
		CheckDlgButton(hWnd, IDC_EXP_MATERIAL, m_pluginCfg->exportMaterial);
		CheckDlgButton(hWnd, IDC_EXP_MODIFIERS, m_pluginCfg->exportModifier);

		CheckDlgButton(hWnd, IDC_EXP_OBJECT_SPACE, m_pluginCfg->objectSpace);
		CheckDlgButton(hWnd, IDC_EXP_BASE_ON_SMGRPS, m_pluginCfg->faceBaseOnSmgrsp);
		CheckDlgButton(hWnd, IDC_EXP_MAPPING_CHANNEL, m_pluginCfg->mappingChannels);
		CheckDlgButton(hWnd, IDC_EXP_VERTEX_NORMAL, m_pluginCfg->vertexNormal);
		CheckDlgButton(hWnd, IDC_EXP_VERTEX_UV, m_pluginCfg->textureCoord);
		CheckDlgButton(hWnd, IDC_EXP_VERTEX_CORLOR, m_pluginCfg->vertexColor);

		CheckDlgButton(hWnd, IDC_EXP_RELATIVE, m_pluginCfg->relative);
		CheckDlgButton(hWnd, IDC_EXP_CONSTRAINTS, m_pluginCfg->constraints);
		CheckDlgButton(hWnd, IDC_EXP_SAMPLECONT, m_pluginCfg->forceSample);
		CheckDlgButton(hWnd, IDC_EXP_QUATERNIONS, m_pluginCfg->quaternions);

		CheckDlgButton(hWnd, IDC_EXP_SKIN, m_pluginCfg->skin);
		CheckDlgButton(hWnd, IDC_EXP_GENMOD, m_pluginCfg->general);


		int ID = IDC_COORD_MAX + m_pluginCfg->coordSystemType;
		CheckRadioButton(hWnd, IDC_COORD_MAX, IDC_COORD_OPENGL, ID);

		ID = IDC_NORMALS_LIST + m_pluginCfg->perFace;
		CheckRadioButton(hWnd, IDC_NORMALS_LIST, IDC_NORMALS_FACE, ID);

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

	void LiarMaxDialogMgr::ChangeHandle(HWND hWnd, WPARAM wParam)
	{
		switch (LOWORD(wParam))
		{
		case IDC_EXP_GEOMETRY:
			m_pluginCfg->exportGeom = IsDlgButtonChecked(hWnd, IDC_EXP_GEOMETRY) > 0 ? true : false;
			SetGeoEnable(hWnd);
			break;
		case IDC_EXP_CONTROLLERS:
			m_pluginCfg->exportController = IsDlgButtonChecked(hWnd, IDC_EXP_CONTROLLERS) > 0 ? true : false;
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
		}
	}

	void LiarMaxDialogMgr::ConfirmHandle(HWND hWnd)
	{
		m_pluginCfg->exportGeom = IsDlgButtonChecked(hWnd, IDC_EXP_GEOMETRY) > 0 ? true : false;
		m_pluginCfg->exportSplines = IsDlgButtonChecked(hWnd, IDC_EXP_SPLINES) > 0 ? true : false;
		m_pluginCfg->exportController = IsDlgButtonChecked(hWnd, IDC_EXP_CONTROLLERS) > 0 ? true : false;
		m_pluginCfg->exportMaterial = IsDlgButtonChecked(hWnd, IDC_EXP_MATERIAL) > 0 ? true : false;
		m_pluginCfg->exportModifier = IsDlgButtonChecked(hWnd, IDC_EXP_MODIFIERS) > 0 ? true : false;

		m_pluginCfg->objectSpace = IsDlgButtonChecked(hWnd, IDC_EXP_OBJECT_SPACE) > 0 ? true : false;
		m_pluginCfg->faceBaseOnSmgrsp = IsDlgButtonChecked(hWnd, IDC_EXP_BASE_ON_SMGRPS) > 0 ? true : false;
		m_pluginCfg->mappingChannels = IsDlgButtonChecked(hWnd, IDC_EXP_MAPPING_CHANNEL) > 0 ? true : false;
		m_pluginCfg->vertexNormal = IsDlgButtonChecked(hWnd, IDC_EXP_VERTEX_NORMAL) > 0 ? true : false;
		m_pluginCfg->textureCoord = IsDlgButtonChecked(hWnd, IDC_EXP_VERTEX_UV) > 0 ? true : false;
		m_pluginCfg->vertexColor = IsDlgButtonChecked(hWnd, IDC_EXP_VERTEX_CORLOR) > 0 ? true : false;

		m_pluginCfg->relative = IsDlgButtonChecked(hWnd, IDC_EXP_RELATIVE) > 0 ? true : false;
		m_pluginCfg->constraints = IsDlgButtonChecked(hWnd, IDC_EXP_CONSTRAINTS) > 0 ? true : false;
		m_pluginCfg->forceSample = IsDlgButtonChecked(hWnd, IDC_EXP_SAMPLECONT) > 0 ? true : false;
		m_pluginCfg->quaternions = IsDlgButtonChecked(hWnd, IDC_EXP_QUATERNIONS) > 0 ? true : false;

		m_pluginCfg->skin = IsDlgButtonChecked(hWnd, IDC_EXP_SKIN) > 0 ? true : false;
		m_pluginCfg->general = IsDlgButtonChecked(hWnd, IDC_EXP_GENMOD) > 0 ? true : false;

		if (IsDlgButtonChecked(hWnd, IDC_COORD_MAX))
		{
			m_pluginCfg->coordSystemType = LiarCoordSysType::Max3DS;
		}
		else if (IsDlgButtonChecked(hWnd, IDC_COORD_OPENGL))
		{
			m_pluginCfg->coordSystemType = LiarCoordSysType::OpenGL;
		}
		else
		{
			m_pluginCfg->coordSystemType = LiarCoordSysType::DirectX;
		}

		ISpinnerControl* spin = GetISpinner(GetDlgItem(hWnd, IDC_STATIC_FRAME));
		m_pluginCfg->staticFrame = spin->GetIVal();
		ReleaseISpinner(spin);

		spin = GetISpinner(GetDlgItem(hWnd, IDC_SAMPLE_FRAME));
		m_pluginCfg->framePerSample = spin->GetIVal();
		ReleaseISpinner(spin);
	}

	void LiarMaxDialogMgr::SetGeoEnable(HWND hWnd)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_OBJECT_SPACE), m_pluginCfg->exportGeom);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_BASE_ON_SMGRPS), m_pluginCfg->exportGeom);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_MAPPING_CHANNEL), m_pluginCfg->exportGeom);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_VERTEX_NORMAL), m_pluginCfg->exportGeom);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_VERTEX_UV), m_pluginCfg->exportGeom);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_VERTEX_CORLOR), m_pluginCfg->exportGeom);
	}

	void LiarMaxDialogMgr::SetControllerEnable(HWND hWnd)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_RELATIVE), m_pluginCfg->exportController);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_CONSTRAINTS), m_pluginCfg->exportController);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_SAMPLECONT), m_pluginCfg->exportController);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_QUATERNIONS), m_pluginCfg->exportController);
	}

	void LiarMaxDialogMgr::SetModifierEnable(HWND hWnd)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_SKIN), m_pluginCfg->exportModifier);
		EnableWindow(GetDlgItem(hWnd, IDC_EXP_GENMOD), m_pluginCfg->exportModifier);
	}

	void LiarMaxDialogMgr::SetVertexNormalEnable(HWND hWnd)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_NORMALS_LIST), m_pluginCfg->vertexNormal);
		EnableWindow(GetDlgItem(hWnd, IDC_NORMALS_FACE), m_pluginCfg->vertexNormal);
	}
}
