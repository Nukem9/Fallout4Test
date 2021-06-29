#include "..\ComClasses.h"

#include <Uxtheme.h>

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			class CUIDWMWindow
			{
			private:
				HWND m_hWnd;
			public:
				static BOOL IsCompositionEnabled(VOID);
				static BOOL ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lResult);
				static BOOL ExtendFrameIntoClientArea(HWND hWnd, const MARGINS* lpMargins);
			public:
				BOOL GetEnabledNonClientRendering(VOID) const;
				VOID SetEnabledNonClientRendering(const BOOL value);
			public:
				BOOL ExtendFrameIntoClientArea(const MARGINS& pMargins) const;
			public:
				CUIDWMWindow(VOID) : m_hWnd(NULL) {}
				CUIDWMWindow(HWND hWnd) : m_hWnd(hWnd) {}
			public:
				PROPERTY(GetEnabledNonClientRendering, SetEnabledNonClientRendering) BOOL EnabledNonClientRendering;
			};
		}
	}
}