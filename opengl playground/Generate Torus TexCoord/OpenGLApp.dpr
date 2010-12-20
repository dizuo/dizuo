
//------------------------------------------------------------------------
//
// Author      : Maarten Kronberger
// Email       : sulacomcclaw@hotmail.com
// Website     : http://www.sulaco.co.za
// Date        : 18 November 2003
// Version     : 1.0
// Description : Automatic Texture-Coordinate Generation
//
//------------------------------------------------------------------------
program OpenGLApp;

uses
  Windows,
  Messages,
  Math,
  OpenGL;

const
  WND_TITLE = 'Automatic Texture-Coordinate Generation by McCLaw (RedBook)';
  FPS_TIMER = 1;                     // Timer to calculate FPS
  FPS_INTERVAL = 1000;               // Calculate FPS every 1000 ms

  // User Constants

  STRIPE_IMAGE_WIDTH = 32;

  sgenparams : array [0..3] of GLfloat = (1.0, 0.0, 0.0, 0.0);


var
  h_Wnd  : HWND;                     // Global window handle
  h_DC   : HDC;                      // Global device context
  h_RC   : HGLRC;                    // OpenGL rendering context
  keys : Array[0..255] of Boolean;   // Holds keystrokes
  FPSCount : Integer = 0;            // Counter for FPS
  ElapsedTime : Integer;             // Elapsed time between frames

  // User Variables

  spin : GLfloat = 0.0;              // Var used for the amount to rotate an object by


  TorusDL : GLuint;                  // For torus DL

   // Mouse
  MouseButton : Integer = -1;         // mouse button down
  xPos, yPos, zPos : glFloat;         // Location
  Xcoord, Ycoord, Zcoord : Integer;   // Mouse Coordinates


  stripeImage : array [0..3*STRIPE_IMAGE_WIDTH] of GLubyte;

{$R *.RES}

{------------------------------------------------------------------}
{  Function to convert int to string. (No sysutils = smaller EXE)  }
{------------------------------------------------------------------}
function IntToStr(Num : Integer) : String;  // using SysUtils increase file size by 100K
begin
  Str(Num, result);
end;


{------------------------------------------------------------------}
{  Calculate the next rotation value an assign it to spin          }
{------------------------------------------------------------------}
procedure SpinDisplay();
begin
    spin := spin + 2.0;
    if spin > 360.0 then
        spin := spin - 360.0;
end;

{------------------------------------------------------------------}
{  Create a red and green striped one dimentional image            }
{------------------------------------------------------------------}
procedure LoadStripeImage;
var i, j : GLint;
begin
    for j := 0 to STRIPE_IMAGE_WIDTH -1 do
    begin
        if j<=4 then
          stripeImage[3*j] := 255
        else
          stripeImage[3*j] := 0;
        if j>4 then
          stripeImage[3*j+1] := 255
        else
          stripeImage[3*j+1] := 0;
        stripeImage[3*j+2] := 0;
    end;
end;

{------------------------------------------------------------------}
{  Substitute for auxWireBox (Draws a box with quads)              }
{------------------------------------------------------------------}
procedure DrawBox(Height, Width, Depth : GLfloat);
var HalfHeight, HalfWidth, HalfDepth : GLfloat;
begin

  HalfHeight := Height/2;
  HalfWidth := Width/2;
  HalfDepth := Depth/2;

  glBegin(GL_QUADS);
    // Front Face
    glNormal3f( 0.0, 0.0, 1.0);
    glVertex3f(-HalfWidth, -HalfHeight,  HalfDepth);
    glVertex3f( HalfWidth, -HalfHeight,  HalfDepth);
    glVertex3f( HalfWidth,  HalfHeight,  HalfDepth);
    glVertex3f(-HalfWidth,  HalfHeight,  HalfDepth);
    // Back Face
    glNormal3f( 0.0, 0.0,-1.0);
    glVertex3f(-HalfWidth, -HalfHeight, -HalfDepth);
    glVertex3f(-HalfWidth,  HalfHeight, -HalfDepth);
    glVertex3f( HalfWidth,  HalfHeight, -HalfDepth);
    glVertex3f( HalfWidth, -HalfHeight, -HalfDepth);
    // Top Face
    glNormal3f( 0.0, 1.0, 0.0);
    glVertex3f(-HalfWidth,  HalfHeight, -HalfDepth);
    glVertex3f(-HalfWidth,  HalfHeight,  HalfDepth);
    glVertex3f( HalfWidth,  HalfHeight,  HalfDepth);
    glVertex3f( HalfWidth,  HalfHeight, -HalfDepth);
    // Bottom Face
    glNormal3f( 0.0,-1.0, 0.0);
    glVertex3f(-HalfWidth, -HalfHeight, -HalfDepth);
    glVertex3f( HalfWidth, -HalfHeight, -HalfDepth);
    glVertex3f( HalfWidth, -HalfHeight,  HalfDepth);
    glVertex3f(-HalfWidth, -HalfHeight,  HalfDepth);
    // Right face
    glNormal3f( 1.0, 0.0, 0.0);
    glVertex3f( HalfWidth, -HalfHeight, -HalfDepth);
    glVertex3f( HalfWidth,  HalfHeight, -HalfDepth);
    glVertex3f( HalfWidth,  HalfHeight,  HalfDepth);
    glVertex3f( HalfWidth, -HalfHeight,  HalfDepth);
    // Left Face
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(-HalfWidth, -HalfHeight, -HalfDepth);
    glVertex3f(-HalfWidth, -HalfHeight,  HalfDepth);
    glVertex3f(-HalfWidth,  HalfHeight,  HalfDepth);
    glVertex3f(-HalfWidth,  HalfHeight, -HalfDepth);
  glEnd();
end;


{------------------------------------------------------------------}
{  Create a torus be giving inner, outer radius and detail level   }
{------------------------------------------------------------------}
procedure CreateTorus(TubeRadius, Radius : GLfloat; Sides, Rings : Integer);
var I, J : Integer;
    theta, phi, theta1 : GLfloat;
    cosTheta, sinTheta : GLfloat;
    cosTheta1, sinTheta1 : GLfloat;
    ringDelta, sideDelta : GLfloat;
    cosPhi, sinPhi, dist : GLfloat;
begin
  sideDelta := 2.0 * Pi / Sides;
  ringDelta := 2.0 * Pi / rings;
  theta := 0.0;
  cosTheta := 1.0;
  sinTheta := 0.0;

  TorusDL :=glGenLists(1);
  glNewList(TorusDL, GL_COMPILE);
    for i := rings - 1 downto 0 do
    begin
      theta1 := theta + ringDelta;
      cosTheta1 := cos(theta1);
      sinTheta1 := sin(theta1);
      glBegin(GL_QUAD_STRIP);
        phi := 0.0;
        for j := Sides downto 0 do
        begin
          phi := phi + sideDelta;
          cosPhi := cos(phi);
          sinPhi := sin(phi);
          dist := Radius + (TubeRadius * cosPhi);

          glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
          glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, TubeRadius * sinPhi);

          glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
          glVertex3f(cosTheta * dist, -sinTheta * dist, TubeRadius * sinPhi);
        end;
      glEnd();
      theta := theta1;
      cosTheta := cosTheta1;
      sinTheta := sinTheta1;
    end;
  glEndList();
end;


{------------------------------------------------------------------}
{  Function to draw the actual scene                               }
{------------------------------------------------------------------}
procedure glDraw();
begin

  glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);// Clear the colour and depth buffer
  glLoadIdentity;                                     // Load a new matrix onto the stack

  glTranslatef(0.0,0.0,-10.0);                        // Move the scene back 10 units
  SpinDisplay;                                        // Calculate the next rotation angle
  glRotatef(spin, 1.0, 0.0, 1.0);                     // Rotate the scene on the x an z-axis
  DrawBox(1.0,1.0,1.0);                               // Draw a cube
  glCallList(TorusDL);                                // Draw a torus

  // Flush the OpenGL Buffer
  glFlush();                        // ( Force the buffer to draw or send a network packet of commands in a networked system)
end;


{------------------------------------------------------------------}
{  Initialise OpenGL                                               }
{------------------------------------------------------------------}
procedure glInit();
begin
  glClearColor (0.0, 0.0, 0.0, 0.0);                           // Set the background colour to black

  LoadStripeImage();                                           // Init the 1D image
  CreateTorus( 0.6, 1.8, 128, 256);                            // Create a Torus
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                       // Set the bit alignment for the stiped image
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Set the texture environment mode to modulate
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S,
      GL_REPEAT);                                              // Tile the texture on the S coordinate
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER,
      GL_LINEAR);                                              // Set the Magnification filter to Linear
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,
      GL_LINEAR);                                              // Set the Minification filter to Linear
  glTexImage1D(GL_TEXTURE_1D, 0, 3, STRIPE_IMAGE_WIDTH, 0,
      GL_RGB, GL_UNSIGNED_BYTE, @stripeImage);                 // Load the texture into video memory
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);      // Generate Linear S texture coordinates
  glTexGenfv(GL_S, GL_OBJECT_PLANE, @sgenparams);              // Set the parameters for the S coordinate on the Object plane
  
  glEnable(GL_TEXTURE_GEN_S);                                  // Enable S texture coordinate generation
  glEnable(GL_TEXTURE_1D);                                     // Enable 1 dimentional textures

  glEnable(GL_DEPTH_TEST);                                     // Enable Depth Testing
  glDepthFunc(GL_LEQUAL);                                      // Define the Depth funcion as Linear Equal
  
  glCullFace(GL_BACK);                                         // Which face do we cull
  glEnable(GL_CULL_FACE);                                      // Enable back face culling
  glEnable(GL_LIGHTING);                                       // Enable Lighting
  glEnable(GL_LIGHT0);                                         // Enable Light 0
  glEnable(GL_AUTO_NORMAL);                                    // Auto generate Normals
  glEnable(GL_NORMALIZE);                                      // Enable Normalization

  glMaterialf (GL_FRONT, GL_SHININESS, 64.0);                  // Set the material shininess for all object drawn in the scene


end;


{------------------------------------------------------------------}
{  Handle window resize                                            }
{------------------------------------------------------------------}
procedure glResizeWnd(Width, Height : Integer);
begin

  if (Height = 0) then                // prevent divide by zero exception
    Height := 1;
  glViewport(0, 0, Width, Height);    // Set the viewport for the OpenGL window
  glMatrixMode(GL_PROJECTION);        // Change Matrix Mode to Projection
  glLoadIdentity();                   // Reset View
  gluPerspective(45.0, Width/Height, 1.0, 20.0);  // Do the perspective calculations. Last value = max clipping depth

  glMatrixMode(GL_MODELVIEW);         // Return to the modelview matrix
  glLoadIdentity();                   // Reset View



end;

{------------------------------------------------------------------}
{  Processes all the mouse clicks                                  }
{------------------------------------------------------------------}
procedure ProcessMouse;
begin
  case MouseButton of
  1: // Left Mouse Button
    begin
      MouseButton := 0; // Cancel our mouse click (To use this procedure as a mouse down event remove this line)
    end;
  2: // Right Mouse Button
    begin
      MouseButton := 0; // Cancel our mouse click (To use this procedure as a mouse down event remove this line)
    end;
  3: // Middle Mouse Button
    begin
      MouseButton := 0; // Cancel our mouse click (To use this procedure as a mouse down event remove this line)
    end;
  end;
end;

{------------------------------------------------------------------}
{  Processes all the keystrokes                                    }
{------------------------------------------------------------------}
procedure ProcessKeys;
begin
  // Reserved for future use
end;


{------------------------------------------------------------------}
{  Determines the application’s response to the messages received  }
{------------------------------------------------------------------}
function WndProc(hWnd: HWND; Msg: UINT;  wParam: WPARAM;  lParam: LPARAM): LRESULT; stdcall;
begin
  case (Msg) of
    WM_CREATE:
      begin
        // Insert stuff you want executed when the program starts
      end;
    WM_CLOSE:
      begin
        PostQuitMessage(0);
        Result := 0
      end;
    WM_KEYDOWN:       // Set the pressed key (wparam) to equal true so we can check if its pressed
      begin
        keys[wParam] := True;
        Result := 0;
      end;
    WM_KEYUP:         // Set the released key (wparam) to equal false so we can check if its pressed
      begin
        keys[wParam] := False;
        Result := 0;
      end;
    WM_LBUTTONDOWN:
      begin
        ReleaseCapture();   // need them here, because if mouse moves off
        SetCapture(h_Wnd);  // window and returns, it needs to reset status
        MouseButton := 1;
        Xcoord := LOWORD(lParam);
        Ycoord := HIWORD(lParam);
        Result := 0;
      end;
    WM_LBUTTONUP:
      begin
        ReleaseCapture();   // above
        MouseButton := 0;
        XCoord := 0;
        YCoord := 0;
        Result := 0;
      end;
    WM_SIZE:          // Resize the window with the new width and height
      begin
        glResizeWnd(LOWORD(lParam),HIWORD(lParam));
        Result := 0;
      end;
    WM_TIMER :                     // Add code here for all timers to be used.
      begin
        if wParam = FPS_TIMER then
        begin
          FPSCount :=Round(FPSCount * 1000/FPS_INTERVAL);   // calculate to get per Second incase intercal is less or greater than 1 second
          SetWindowText(h_Wnd, PChar(WND_TITLE + '   [' + intToStr(FPSCount) + ' FPS]'));
          FPSCount := 0;
          Result := 0;
        end;
      end;
    else
      Result := DefWindowProc(hWnd, Msg, wParam, lParam);    // Default result if nothing happens
  end;
end;


{---------------------------------------------------------------------}
{  Properly destroys the window created at startup (no memory leaks)  }
{---------------------------------------------------------------------}
procedure glKillWnd(Fullscreen : Boolean);
begin
  if Fullscreen then             // Change back to non fullscreen
  begin
    ChangeDisplaySettings(devmode(nil^), 0);
    ShowCursor(True);
  end;

  // Makes current rendering context not current, and releases the device
  // context that is used by the rendering context.
  if (not wglMakeCurrent(h_DC, 0)) then
    MessageBox(0, 'Release of DC and RC failed!', 'Error', MB_OK or MB_ICONERROR);

  // Attempts to delete the rendering context
  if (not wglDeleteContext(h_RC)) then
  begin
    MessageBox(0, 'Release of rendering context failed!', 'Error', MB_OK or MB_ICONERROR);
    h_RC := 0;
  end;

  // Attemps to release the device context
  if ((h_DC > 0) and (ReleaseDC(h_Wnd, h_DC) = 0)) then
  begin
    MessageBox(0, 'Release of device context failed!', 'Error', MB_OK or MB_ICONERROR);
    h_DC := 0;
  end;

  // Attempts to destroy the window
  if ((h_Wnd <> 0) and (not DestroyWindow(h_Wnd))) then
  begin
    MessageBox(0, 'Unable to destroy window!', 'Error', MB_OK or MB_ICONERROR);
    h_Wnd := 0;
  end;

  // Attempts to unregister the window class
  if (not UnRegisterClass('OpenGL', hInstance)) then
  begin
    MessageBox(0, 'Unable to unregister window class!', 'Error', MB_OK or MB_ICONERROR);
    hInstance := 0;
  end;
end;


{--------------------------------------------------------------------}
{  Creates the window and attaches a OpenGL rendering context to it  }
{--------------------------------------------------------------------}
function glCreateWnd(Width, Height : Integer; Fullscreen : Boolean; PixelDepth : Integer) : Boolean;
var
  wndClass : TWndClass;         // Window class
  dwStyle : DWORD;              // Window styles
  dwExStyle : DWORD;            // Extended window styles
  dmScreenSettings : DEVMODE;   // Screen settings (fullscreen, etc...)
  PixelFormat : GLuint;         // Settings for the OpenGL rendering
  h_Instance : HINST;           // Current instance
  pfd : TPIXELFORMATDESCRIPTOR;  // Settings for the OpenGL window
begin
  h_Instance := GetModuleHandle(nil);       //Grab An Instance For Our Window
  ZeroMemory(@wndClass, SizeOf(wndClass));  // Clear the window class structure

  with wndClass do                    // Set up the window class
  begin
    style         := CS_HREDRAW or    // Redraws entire window if length changes
                     CS_VREDRAW or    // Redraws entire window if height changes
                     CS_OWNDC;        // Unique device context for the window
    lpfnWndProc   := @WndProc;        // Set the window procedure to our func WndProc
    hInstance     := h_Instance;
    hCursor       := LoadCursor(0, IDC_ARROW);
    lpszClassName := 'OpenGL';
  end;

  if (RegisterClass(wndClass) = 0) then  // Attemp to register the window class
  begin
    MessageBox(0, 'Failed to register the window class!', 'Error', MB_OK or MB_ICONERROR);
    Result := False;
    Exit
  end;

  // Change to fullscreen if so desired
  if Fullscreen then
  begin
    ZeroMemory(@dmScreenSettings, SizeOf(dmScreenSettings));
    with dmScreenSettings do begin              // Set parameters for the screen setting
      dmSize       := SizeOf(dmScreenSettings);
      dmPelsWidth  := Width;                    // Window width
      dmPelsHeight := Height;                   // Window height
      dmBitsPerPel := PixelDepth;               // Window color depth
      dmFields     := DM_PELSWIDTH or DM_PELSHEIGHT or DM_BITSPERPEL;
    end;

    // Try to change screen mode to fullscreen
    if (ChangeDisplaySettings(dmScreenSettings, CDS_FULLSCREEN) = DISP_CHANGE_FAILED) then
    begin
      MessageBox(0, 'Unable to switch to fullscreen!', 'Error', MB_OK or MB_ICONERROR);
      Fullscreen := False;
    end;
  end;

  // If we are still in fullscreen then
  if (Fullscreen) then
  begin
    dwStyle := WS_POPUP or                // Creates a popup window
               WS_CLIPCHILDREN            // Doesn't draw within child windows
               or WS_CLIPSIBLINGS;        // Doesn't draw within sibling windows
    dwExStyle := WS_EX_APPWINDOW;         // Top level window
    ShowCursor(False);                    // Turn of the cursor (gets in the way)
  end
  else
  begin
    dwStyle := WS_OVERLAPPEDWINDOW or     // Creates an overlapping window
               WS_CLIPCHILDREN or         // Doesn't draw within child windows
               WS_CLIPSIBLINGS;           // Doesn't draw within sibling windows
    dwExStyle := WS_EX_APPWINDOW or       // Top level window
                 WS_EX_WINDOWEDGE;        // Border with a raised edge
  end;

  // Attempt to create the actual window
  h_Wnd := CreateWindowEx(dwExStyle,      // Extended window styles
                          'OpenGL',       // Class name
                          WND_TITLE,      // Window title (caption)
                          dwStyle,        // Window styles
                          0, 0,           // Window position
                          Width, Height,  // Size of window
                          0,              // No parent window
                          0,              // No menu
                          h_Instance,     // Instance
                          nil);           // Pass nothing to WM_CREATE
  if h_Wnd = 0 then
  begin
    glKillWnd(Fullscreen);                // Undo all the settings we've changed
    MessageBox(0, 'Unable to create window!', 'Error', MB_OK or MB_ICONERROR);
    Result := False;
    Exit;
  end;

  // Try to get a device context
  h_DC := GetDC(h_Wnd);
  if (h_DC = 0) then
  begin
    glKillWnd(Fullscreen);
    MessageBox(0, 'Unable to get a device context!', 'Error', MB_OK or MB_ICONERROR);
    Result := False;
    Exit;
  end;

  // Settings for the OpenGL window
  with pfd do
  begin
    nSize           := SizeOf(TPIXELFORMATDESCRIPTOR); // Size Of This Pixel Format Descriptor
    nVersion        := 1;                    // The version of this data structure
    dwFlags         := PFD_DRAW_TO_WINDOW    // Buffer supports drawing to window
                       or PFD_SUPPORT_OPENGL // Buffer supports OpenGL drawing
                       or PFD_DOUBLEBUFFER;  // Supports double buffering
    iPixelType      := PFD_TYPE_RGBA;        // RGBA color format
    cColorBits      := PixelDepth;           // OpenGL color depth
    cRedBits        := 0;                    // Number of red bitplanes
    cRedShift       := 0;                    // Shift count for red bitplanes
    cGreenBits      := 0;                    // Number of green bitplanes
    cGreenShift     := 0;                    // Shift count for green bitplanes
    cBlueBits       := 0;                    // Number of blue bitplanes
    cBlueShift      := 0;                    // Shift count for blue bitplanes
    cAlphaBits      := 0;                    // Not supported
    cAlphaShift     := 0;                    // Not supported
    cAccumBits      := 0;                    // No accumulation buffer
    cAccumRedBits   := 0;                    // Number of red bits in a-buffer
    cAccumGreenBits := 0;                    // Number of green bits in a-buffer
    cAccumBlueBits  := 0;                    // Number of blue bits in a-buffer
    cAccumAlphaBits := 0;                    // Number of alpha bits in a-buffer
    cDepthBits      := 16;                   // Specifies the depth of the depth buffer
    cStencilBits    := 0;                    // Turn off stencil buffer
    cAuxBuffers     := 0;                    // Not supported
    iLayerType      := PFD_MAIN_PLANE;       // Ignored
    bReserved       := 0;                    // Number of overlay and underlay planes
    dwLayerMask     := 0;                    // Ignored
    dwVisibleMask   := 0;                    // Transparent color of underlay plane
    dwDamageMask    := 0;                     // Ignored
  end;

  // Attempts to find the pixel format supported by a device context that is the best match to a given pixel format specification.
  PixelFormat := ChoosePixelFormat(h_DC, @pfd);
  if (PixelFormat = 0) then
  begin
    glKillWnd(Fullscreen);
    MessageBox(0, 'Unable to find a suitable pixel format', 'Error', MB_OK or MB_ICONERROR);
    Result := False;
    Exit;
  end;

  // Sets the specified device context's pixel format to the format specified by the PixelFormat.
  if (not SetPixelFormat(h_DC, PixelFormat, @pfd)) then
  begin
    glKillWnd(Fullscreen);
    MessageBox(0, 'Unable to set the pixel format', 'Error', MB_OK or MB_ICONERROR);
    Result := False;
    Exit;
  end;

  // Create a OpenGL rendering context
  h_RC := wglCreateContext(h_DC);
  if (h_RC = 0) then
  begin
    glKillWnd(Fullscreen);
    MessageBox(0, 'Unable to create an OpenGL rendering context', 'Error', MB_OK or MB_ICONERROR);
    Result := False;
    Exit;
  end;

  // Makes the specified OpenGL rendering context the calling thread's current rendering context
  if (not wglMakeCurrent(h_DC, h_RC)) then
  begin
    glKillWnd(Fullscreen);
    MessageBox(0, 'Unable to activate OpenGL rendering context', 'Error', MB_OK or MB_ICONERROR);
    Result := False;
    Exit;
  end;

  // Initializes the timer used to calculate the FPS
  SetTimer(h_Wnd, FPS_TIMER, FPS_INTERVAL, nil);

  // Settings to ensure that the window is the topmost window
  ShowWindow(h_Wnd, SW_SHOW);
  SetForegroundWindow(h_Wnd);
  SetFocus(h_Wnd);

  // Ensure the OpenGL window is resized properly
  glResizeWnd(Width, Height);
  glInit(); // Initialise any OpenGL States and variables 

  Result := True;
end;


{--------------------------------------------------------------------}
{  Main message loop for the application                             }
{--------------------------------------------------------------------}
function WinMain(hInstance : HINST; hPrevInstance : HINST;
                 lpCmdLine : PChar; nCmdShow : Integer) : Integer; stdcall;
var
  msg : TMsg;
  finished : Boolean;
  DemoStart, LastTime : DWord;
begin
  finished := False;

  // Perform application initialization:
  if not glCreateWnd(800, 600, FALSE, 32) then
  begin
    Result := 0;
    Exit;
  end;

  DemoStart := GetTickCount();            // Get Time when demo started

  // Main message loop:
  while not finished do
  begin
    if (PeekMessage(msg, 0, 0, 0, PM_REMOVE)) then // Check if there is a message for this window
    begin
      if (msg.message = WM_QUIT) then     // If WM_QUIT message received then we are done
        finished := True
      else
      begin                               // Else translate and dispatch the message to this window
  	TranslateMessage(msg);
        DispatchMessage(msg);
      end;
    end
    else
    begin
      Inc(FPSCount);                      // Increment FPS Counter

      LastTime :=ElapsedTime;
      ElapsedTime :=GetTickCount() - DemoStart;     // Calculate Elapsed Time
      ElapsedTime :=(LastTime + ElapsedTime) DIV 2; // Average it out for smoother movement

      glDraw();                           // Draw the scene ( Call any OpenGL Rendering code in this function)
      SwapBuffers(h_DC);                  // Display the scene

      if (keys[VK_ESCAPE]) then           // If user pressed ESC then set finised TRUE
        finished := True
      else
        ProcessKeys;                      // Check for any other key Pressed
        ProcessMouse;                     // Check for mouse clicks
    end;
  end;
  glKillWnd(FALSE);
  Result := msg.wParam;
end;


begin
  WinMain( hInstance, hPrevInst, CmdLine, CmdShow );
end.
