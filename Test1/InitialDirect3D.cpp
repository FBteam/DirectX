//-----------------------------------------------------------------------------------
// InitialDirect3D.CPP 
// Инициализируем Direct3D
//-----------------------------------------------------------------------------------

#include <windows.h>         // Подключаем заголовочный файл Windows
#include <d3d9.h>            // Подключаем заголовочный файл DirectX 9 SDK

#pragma comment(lib, "d3d9.lib")


LPDIRECT3D9                 pDirect3D          = NULL;       
LPDIRECT3DDEVICE9           pDirect3DDevice    = NULL;       
LPDIRECT3DVERTEXBUFFER9     pBufferVershin     = NULL;      

struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; //кординаты 
    DWORD color;        //цвет вершин
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

//-----------------------------------------------------------------------------------
// Функция
// InitialDirect3D()
// Инициализация Direct3D
//-----------------------------------------------------------------------------------

HRESULT InitialDirect3D( HWND hwnd )
{
    
    if( NULL == ( pDirect3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    D3DDISPLAYMODE Display;
    if( FAILED( pDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &Display ) ) )
        return E_FAIL;

    D3DPRESENT_PARAMETERS Direct3DParametr; 
    ZeroMemory( &Direct3DParametr, sizeof(Direct3DParametr) );
    Direct3DParametr.Windowed         = TRUE;
    Direct3DParametr.SwapEffect       = D3DSWAPEFFECT_DISCARD;
    Direct3DParametr.BackBufferFormat = Display.Format;

    if( FAILED( pDirect3D -> CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &Direct3DParametr, &pDirect3DDevice ) ) )
    {
        return E_FAIL;
    }

    return S_OK;
}
//---------------------------------------------------------------------------------------
// Функция
// InitialBufferVershin()
// Иннициализирует вершины
//---------------------------------------------------------------------------------------

HRESULT InitialBufferVershin()
{
   
    CUSTOMVERTEX Vershin[] =
    {
        { 300.0f, 300.0f, 0.5f, 1.0f, 0x00000fff, },  // А
        { 150.0f, 300.0f, 0.5f, 1.0f, 0x00000fff, },  // В
        { 150.0f, 150.0f, 0.5f, 1.0f, 0x00000fff, },  // С
		//  x       y      z    rhw      цвет

        { 150.0f, 150.0f, 0.5f, 1.0f, 0x00000fff, },  // C
        { 300.0f, 150.0f, 0.5f, 1.0f, 0x00000fff, },  // D
        { 300.0f, 300.0f, 0.5f, 1.0f, 0x00000fff, },  // A
		//  x       y      z    rhw      цвет
    };

    if( FAILED( pDirect3DDevice->CreateVertexBuffer( 6*sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &pBufferVershin, NULL ) ) )
    {
        return E_FAIL;
    }
   
    VOID* pBV;
    if( FAILED( pBufferVershin->Lock( 0, sizeof(Vershin), (void**)&pBV, 0 ) ) ) //Блокируем
        return E_FAIL;

    memcpy( pBV, Vershin, sizeof(Vershin) ); //Копируем
    pBufferVershin->Unlock();                // Разблокируем

    return S_OK;
}

//-----------------------------------------------------------------------------------
//  Функция
//  RenderingDirect3D()
//  Рисуем
//-----------------------------------------------------------------------------------

VOID RenderingDirect3D()
{
    
    if(pDirect3DDevice == NULL)        // Проверяем ошибки
        return;
    
	
    pDirect3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,255,255), 1.0f, 0 );
    
    pDirect3DDevice->BeginScene();     // Начало сцены

    //  Здесь происходит прорисовка сцены
    pDirect3DDevice->SetStreamSource( 0, pBufferVershin, 0, sizeof(CUSTOMVERTEX) );
    pDirect3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

    pDirect3DDevice->EndScene();       //Конец сцены
    
    pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
	  
}

//-----------------------------------------------------------------------------------
//  Функция
//  DeleteDirect3D()
//  Освобождает захваченные ресурсы
//-----------------------------------------------------------------------------------

VOID DeleteDirect3D()
{

    if( pBufferVershin  != NULL)
        pBufferVershin->Release(); 

    if( pDirect3DDevice != NULL) 
        pDirect3DDevice->Release();

    if( pDirect3D != NULL)
        pDirect3D->Release();
}

//-----------------------------------------------------------------------------------
// Функция
// MainWinProc()
// Здесь происходит обработка сообщений
//-----------------------------------------------------------------------------------

LRESULT CALLBACK MainWinProc(HWND   hwnd,            
					        UINT    msg,              
                            WPARAM  wparam,           
                            LPARAM  lparam)          
{

switch(msg)
	{	
	
     case WM_DESTROY: 
		{
		DeleteDirect3D();
		PostQuitMessage(0);
		return(0);
		} 
	} 

  
return DefWindowProc(hwnd, msg, wparam, lparam);

} 

//-----------------------------------------------------------------------------------
// Функция
// WinMain
// Входная точка приложения
//-----------------------------------------------------------------------------------

int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

WNDCLASSEX windowsclass;  // Создаем класс
HWND	   hwnd;	      // Создаем дескриптор окна
MSG        msg;           // Идентификатор сообщения

// Определим класс окна WNDCLASSEX
windowsclass.cbSize         = sizeof(WNDCLASSEX);
windowsclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
windowsclass.lpfnWndProc	= MainWinProc;
windowsclass.cbClsExtra		= 0;
windowsclass.cbWndExtra		= 0;
windowsclass.hInstance		= hinstance;
windowsclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
windowsclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
windowsclass.hbrBackground	= (HBRUSH)GetStockObject(GRAY_BRUSH);
windowsclass.lpszMenuName	= NULL;
windowsclass.lpszClassName	= L"WINDOWSCLASS";
windowsclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

// Зарегистрируем класс
if (!RegisterClassEx(&windowsclass))
	return(0);

// Теперь когда класс зарегестрирован можно создать окно

if (!(hwnd = CreateWindowEx(NULL,              // стиль окна
                       L"WINDOWSCLASS",         // класс
					   L"Урок 4. Рисуем Квадрат ",     // название окна
					   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					   300,150,	               // левый верхний угол
					   500,400,                // ширина и высота
					   NULL,	               // дескриптор родительского окна 
					   NULL,	               // дескриптор меню
					   hinstance,              // дескриптор экземпляра приложения
					   NULL)))	               // указатель на данные окна                                            
return 0;
     
     
 if( SUCCEEDED( InitialDirect3D( hwnd ) ) )
  { 
       if( SUCCEEDED( InitialBufferVershin( ) ) )
	    {
           ShowWindow( hwnd, SW_SHOWDEFAULT );
           UpdateWindow( hwnd );

              ZeroMemory( &msg, sizeof(msg));
              while( msg.message!=WM_QUIT) 
                {
                  if(PeekMessage( &msg, NULL,0,0,PM_REMOVE ))
		           {
			          TranslateMessage( &msg );
                      DispatchMessage( &msg );
                   }
                  else
                  RenderingDirect3D();
                } 
        }
  } 
   return 0;
}