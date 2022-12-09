// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include <microsoft.ui.xaml.window.h>
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::App1::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        auto windowNative{ this->try_as<::IWindowNative>() };
        windowNative->get_WindowHandle(&hWnd);
        SetWindowPos(hWnd, NULL, 500, 200, 930, 610, NULL);
        this->Title(L"Andong Nation University");
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

}

void winrt::App1::implementation::MainWindow::CanvasControl_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args)
{
    CanvasControl canvas = sender.as<CanvasControl>();
    int n = vx.size();
    for (int i = 1; i < n; i++)
    {
        if (vx[i] == 0 && vy[i] == 0)
        {
            i++; continue;
        }
        args.DrawingSession().DrawLine(vx[i - 1], vy[i - 1], vx[i], vy[i], col[i], size[i]);
        args.DrawingSession().FillCircle(vx[i - 1], vy[i - 1], size[i] / 2, col[i]);
        args.DrawingSession().FillCircle(vx[i], vy[i], size[i] / 2, col[i]);
    }
    canvas.Invalidate();
}


void winrt::App1::implementation::MainWindow::CanvasControl_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    flag = true;
}


void winrt::App1::implementation::MainWindow::CanvasControl_PointerReleased(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    flag = false;
    px = py = 0.0;
    vx.push_back(px);
    vy.push_back(py);
    col.push_back(myCol);
    size.push_back(mySize);
}


void winrt::App1::implementation::MainWindow::CanvasControl_PointerMoved(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    CanvasControl canvas = sender.as<CanvasControl>();
    px = e.GetCurrentPoint(canvas).Position().X;
    py = e.GetCurrentPoint(canvas).Position().Y;
    if (flag) {
        vx.push_back(px);
        vy.push_back(py);
        col.push_back(myCol);
        size.push_back(mySize);
        canvas.Invalidate();
    }
}


void winrt::App1::implementation::MainWindow::togglebtn_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    if (IsChecked2)
    {
        IsChecked2 = FALSE;
        togglebtn().Label(L"OFF");

        colorPanel().Visibility(Visibility::Collapsed);
        SetWindowPos(hWnd, NULL, 500, 200, 615, 610, NULL);
    }
    else
    {
        IsChecked2 = TRUE;
        togglebtn().Label(L"ON");

        colorPanel().Visibility(Visibility::Visible);
        SetWindowPos(hWnd, NULL, 500, 200, 930, 610, NULL);
    }
}


void winrt::App1::implementation::MainWindow::ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args)
{
    myCol = args.NewColor();
}


void winrt::App1::implementation::MainWindow::Slider_ValueChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs const& e)
{
    mySize = e.NewValue();
    txtSize().Text(L"size : " + to_hstring(mySize));
}


void winrt::App1::implementation::MainWindow::Savebtn_Click(winrt::Windows::Foundation::IInspectable const& , winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
{
    FILE* fp;
    fopen_s(&fp, "C:/Users/sy/Desktop/1.txt", "w");

    if (fp)
    {
        int n = vx.size();
        for (int i = 1; i < n; i++)
        {
            fprintf(fp, "%d %d %d %d %d %d\n", vx[i], vy[i], size[i], col[i].R, col[i].G, col[i].B);
        }
        fclose(fp);
    }
}


void winrt::App1::implementation::MainWindow::Loadbtn_Click(winrt::Windows::Foundation::IInspectable const& , winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
{
    FILE* fp;
    fopen_s(&fp, "C:/Users/sy/Desktop/1.txt", "r");

    if (fp)
    {
        vx.clear(); vy.clear(); size.clear(); col.clear();

        int x, y, s, r, g, b;
        while (EOF != fscanf_s(fp, "%d %d %d %d %d %d\n", &x, &y, &s, &r, &g, &b))
        {
            vx.push_back(x);
            vy.push_back(y);
            size.push_back(s);

            Windows::UI::Color tempCol;
            tempCol.R = r; tempCol.G = g; tempCol.B = b;
            col.push_back(tempCol);
        }
        fclose(fp);
    }
}


void winrt::App1::implementation::MainWindow::Clearbtn_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    vx.clear();
    vy.clear();
    size.clear();
    col.clear();
}


void winrt::App1::implementation::MainWindow::Exitbtn_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    this->Close();
}
