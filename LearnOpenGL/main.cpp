#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

#define __APPLE__

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";


//窗宽回调函数，（）中参数的结构由 GLFW 定义不可乱写
void framebuffer_size_callback_func(GLFWwindow *window, int width, int height) {
    //OpenGL中的函数，通过 glad 管理，用于设置窗口的维度
    cout << "调用framebuffer_size_callback_func" << endl;
    glViewport(0, 0, 800, 400);
}

//输入控制，将输入打包在这个函数中处理
void processInput(GLFWwindow *window) {
//    cout << "调用processInput" << endl;
    //时候按下了 ESC 键
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);//改变该对象的关闭标志位
    }
}
//窗宽回调函数声明（）内参数结构不可变

int main() {
/*--------------------------------1.分割线:设置 GLFW 参数----------------------------------------------*/
    glfwInit();//初始化 glfw 窗口

    //This function sets hints for the next call to glfwCreateWindow.
    //告诉 GLFW 我们要使用的OpenGL版本是3.3。
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号(Major)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号(Minor)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//告诉 GLFW 我们使用的是核心模式(Core-profile)
#ifdef __APPLE__
    //mac os需要加上这一条配置
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

/*--------------------------------2.分割线：创建窗口对象----------------------------------------------*/
    GLFWwindow *window_00;//声明一个窗口对象
    window_00 = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "我的窗口", NULL, NULL);//创建GLFW中的窗口对象
    if (window_00 == NULL) {
        cout << "窗口创建失败" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window_00);//将创建的窗口对象绑定到OpenGL上下文中

/*--------------------------------3.分割线：注册窗宽回调函数----------------------------------------------*/
    //向 GLFW 注册一个在对象 window_00 的窗宽发生变化时的回调函数
    glfwSetFramebufferSizeCallback(window_00, framebuffer_size_callback_func);

/*--------------------------------4.分割线：调用 GLAD 管理所有 OpenGL 中的函数指针----------------------------------------------*/
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "GLAD初始化失败" << endl;
        return -1;
    }

/*--------------------------------6.分割线：编写着色器（build and compile our shader program)----------------------------------------------*/
    //----------------------------vertex shader
    //声明一个变量来保存创建的着色器对象的编号；
    unsigned int vertex_Shader = glCreateShader(GL_VERTEX_SHADER);
    //将开头保存为字符串的 GLSL 着色器代码绑定到着色器对象中，并编译它。
    glShaderSource(vertex_Shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_Shader);
    //检查时候编译成功
    int is_Success = 0;
    char info_Log[512];
    glGetShaderiv(vertex_Shader, GL_COMPILE_STATUS, &is_Success);
    if (!is_Success) {
        glGetShaderInfoLog(vertex_Shader, 512, NULL, info_Log);
        cout << "vertex_Shader 编译失败！\n" << info_Log << endl;
    }
    //----------------------------fragment shader
    //声明一个变量来保存创建的着色器对象的编号；「过程同上 vertex_Shader」
    unsigned int fragment_Shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_Shader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_Shader);
    glGetShaderiv(fragment_Shader, GL_COMPILE_STATUS, &is_Success);
    if (!is_Success) {
        glGetShaderInfoLog(fragment_Shader, 512, NULL, info_Log);
        cout << "fragment_Shader 编译失败！" << info_Log << endl;
    }
/*--------------------------------7.将刚创建的两个着色器对象链接（link）到着色器程序（Shader Program）中才能渲染----------------------------------------------*/
    //创建「着色程序」对象
    unsigned int shader_Program = glCreateProgram();
    //Attach着色器到刚创建的着色程序中,并链接他们
    glAttachShader(shader_Program, vertex_Shader);
    glAttachShader(shader_Program, fragment_Shader);
    glLinkProgram(shader_Program);
    //检测时候链接成功
    glGetProgramiv(shader_Program, GL_COMPILE_STATUS, &is_Success);
    if(!is_Success){
        glGetProgramInfoLog(shader_Program, 512, NULL, info_Log);
        cout << "shader_Program 链接错误！\n" << info_Log << endl;
    }
    //链接结束后，在 opengl 中删去已绑定的两个着色器
    glDeleteShader(vertex_Shader);
    glDeleteShader(fragment_Shader);
/*--------------------------------8.输入顶点信息「用 VBO 管理」、配置如何读取「用 VAO 管理」----------------------------------------------*/
    //顶点数组
    float vertices_Array[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f, 0.5f, 0.0f  // top
    };

    //创建 VAO、VBO两个对象-----------
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1,&VAO);
    //先将 VAO 绑定到 Opengl 上，再设置相应的 VBO 属性
    glBindVertexArray(VAO);

    //绑定 VBO 到 Opengl 的 GL_ARRAY_BUFFER 类型的缓冲上，GL 有很多不同类型的缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //将定义的数据复制到显卡能快速访问的缓冲区中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Array), vertices_Array, GL_STATIC_DRAW);
    //配置顶点信息读取方式，供 VAO 使用-------------
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);//0作为启用顶点位置属性一个 index 传入供 OpenGL 使用
    //由于此时 VAO 仍然绑定着，VAO 中储存着 VBO 的 index，所以此时可以解除 VBO 的绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);//0就是解除绑定
    //由于已经配置好了 VAO 所以此时也可以解除 VAO 的绑定，防止配置其他的 VAO 时意外地修改了当前的 VAO
    //但是每次修改 VAO 要求调用glVertexAttribPointer()函数，所以通常我们不需要直接的写下面这行代码来解除 VAO 绑定
    glBindVertexArray(0);
/*--------------------------------5.分割线：渲染循环（Render Loop）----------------------------------------------*/
    //时候采用线框模式绘图
#ifdef _LINE_
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    //检查 window_00 对象中的关闭标志位
    while (!glfwWindowShouldClose(window_00)) {
        //处理输入
        processInput(window_00);

        //渲染「绘图」过程
        glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);
/*--------------------------------9.使用 shader_Program 进行渲染----------------------------------------------*/
        glUseProgram(shader_Program);//激活着色器
        glBindVertexArray(VAO);//不用每次都绑定，但是绑定可以使得代码更易读
        //glDrawArrays它使用当前激活的着色器,之前定义的顶点属性配置,和VBO的顶点数据(通过VAO间接绑定)来绘制图元。
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //将渲染的输出显示在屏幕上
        glfwSwapBuffers(window_00);//glfwSwapBuffers( )会交换颜色缓冲「一个储存着GLFW窗口每个像素颜色值的大缓冲」，它在这一次循环中被用来绘制作为输出显示在屏幕上。

        //检查事件，并调用对应的回调函数
        glfwPollEvents();//glfwPollEvents( )检查有没有触发什么事件「键盘输入、鼠标移动等」，更新窗户口状态，并调用对应的回调函数。
    }

/*--------------------------------6.分割线：结束程序，回收资源----------------------------------------------*/
    //渲染循环结束，删除创建 VAO VBO对象，释放资源
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteShader(shader_Program);
    //释放所有 GLFW 的资源
    glfwTerminate();
    return 0;
}