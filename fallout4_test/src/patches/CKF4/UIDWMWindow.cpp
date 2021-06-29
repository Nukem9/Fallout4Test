#include <dwmapi.h>

#include "UIDWMWindow.h"
#include "UIGraphics.h"

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			BOOL CUIDWMWindow::IsCompositionEnabled(VOID)
			{
				BOOL bEnabled = FALSE;
				if (DwmIsCompositionEnabled(&bEnabled) != S_OK)
					return FALSE;
				return bEnabled;
			}

			BOOL CUIDWMWindow::GetEnabledNonClientRendering(VOID) const
			{
				DWMNCRENDERINGPOLICY ncrp;
				DwmGetWindowAttribute(m_hWnd, DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
				return ncrp == DWMNCRP_ENABLED;
			}

			VOID CUIDWMWindow::SetEnabledNonClientRendering(const BOOL value)
			{
				DWMNCRENDERINGPOLICY ncrp;

				if (value)
					ncrp = DWMNCRP_ENABLED;
				else
					ncrp = DWMNCRP_DISABLED;

				DwmSetWindowAttribute(m_hWnd, DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
			}

			BOOL CUIDWMWindow::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lResult)
			{
				return DwmDefWindowProc(hWnd, uMsg, wParam, lParam, lResult);
			}

			BOOL CUIDWMWindow::ExtendFrameIntoClientArea(HWND hWnd, const MARGINS* lpMargins)
			{
				return DwmExtendFrameIntoClientArea(hWnd, lpMargins) == S_OK;
			}

			BOOL CUIDWMWindow::ExtendFrameIntoClientArea(const MARGINS& pMargins) const
			{
				return ExtendFrameIntoClientArea(m_hWnd, &pMargins);
			}
		}
	}
}