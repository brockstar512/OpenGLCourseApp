#include "MyWindow.h"

MyWindow::MyWindow()
{
    width = 800;
    height = 600;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }

    xChange = 0.0f;
    yChange = 0.0f;
}

MyWindow::MyWindow(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }

    xChange = 0.0f;
    yChange = 0.0f;
}

int MyWindow::Init()
{

    //initialize glfw
    if (!glfwInit())
    {
        printf("GLFW initialization failed");
        glfwTerminate();
        return 1;
    }
    //set up glfw window properties
    // 
    //opengl version... we are using 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //preventing any deprecated libs to be used... no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //allowing forwards compatabilty
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //create window
    window = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    if (!window)
    {
        printf("GLFW window create failed");
        glfwTerminate();
        return 1;
    }

    //window address, buffer width and height address
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    //set context for GLEW to use... if we have multiple windows we can switch between windows to identify the context
    glfwMakeContextCurrent(window);

    //handle key + mouse input
    CreateCallbacks();

    //lock mouse to screen
    glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //allow modern extension features..
    glewExperimental = GL_TRUE;

    //initialize glew
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initiazlietion failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    //initailze depth test so the screen can determine which vertext should be behind vs in front
    glEnable(GL_DEPTH_TEST);

    //setup our viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);


    //**SETTING THE WINDOW**
    //asssign a user pointer to the window being used
    glfwSetWindowUserPointer(window,this);
}

GLfloat MyWindow::GetXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat MyWindow::GetYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

MyWindow::~MyWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void MyWindow::CreateCallbacks()
{
    //when the key is pressed on a window call this function;
    glfwSetKeyCallback(window, HandleKeys);
    glfwSetCursorPosCallback(window,HandleMouse);
}

void MyWindow::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    //get the window that called the static function
    MyWindow* theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}


void MyWindow::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
    MyWindow* theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMove)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMove = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    //make sure its not invertted
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

}
