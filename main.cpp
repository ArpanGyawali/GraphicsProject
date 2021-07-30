#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include "Line.h"
#include "Vec3.h"
#include "Mat4.h"
#include "Cube.h"
#include "ConvertToScreen.h"
#include "DrawTriangle.h"
//#include "Light.h"
#include "Projection.h"
//#include "Model.h"

#define WIDTH 1000
#define HEIGHT 1000

static constexpr float delta = PI;
//float s = 30.0f;
float offsetZ = 2.0f;
float pitchX = 0.0f;
float yawY = 0.0f;
float rollZ = 0.0f;
float Ns = 200.0f;


const Color colors[12] = {
        WHITE,
        WHITE,
        RED,
        RED,
        YELLOW,
        YELLOW,
        PURPLE,
        PURPLE,
        ORANGE,
        ORANGE,
        BLUE,
        BLUE
};
void myinit(void)
{
    //for movement speed
    //float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    /*deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;*/

    glClearColor(0.1, 0.1, 0.1, 0.0);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
    //glMatrixMode(GL_PROJECTION);
    //gluOrtho2D(0, WIDTH, 0, HEIGHT);
    //gluOrtho2D(0, 1, 0, 1);
    gluOrtho2D(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2);
}


void drawCube()
{
    ConvertToScreen screen;
    Cube cube(0.4);
    Vec3f view = Vec3f(0.0f, 0.0f, 1.0f);
    Vec3f reference = Vec3f(0.0f, 0.0f, -1.0f);
    Vec3f Ka = Vec3f(1.0f, 1.0f, 1.0f);
    Vec3f Kd = Vec3f(0.8f, 0.8f, 0.8f);
    Vec3f Ks = Vec3f(0.5f, 0.5f, 0.5f);
    Vec3f light = Vec3f(50.0f, 150.0f, 80.0f);
    Vec3f Ia = Vec3f(0.5f, 0.5f, 0.5f);
    Vec3f Il = Vec3f(0.7f, 0.7f, 0.7f);

    //auto lines = cube.GetLines();
    auto triangles = cube.GetTriangles();
    //Mat4f scale = Mat4f::Scaling(s);
    Mat4f rotation =
        Mat4f::RotationX(pitchX) *
        Mat4f::RotationY(yawY) *
        Mat4f::RotationZ(rollZ);
    Mat4f translate = Mat4f::Translation(0.0f, 0.0f, offsetZ);
    //Mat4f perspective = Mat4f::PerspectiveFOV(120, WIDTH/HEIGHT, 0.7, -0.5);
    Mat4f composite = translate * rotation;
    //Transform from model space to view space
    for (auto& v : triangles.vertices)
    {
        v = composite * v;
        //v = v + Vec3f(0.0f, 0.0f, offsetZ);
        screen.Transform(v);
        PerspectiveView(view.z, reference, v);
    }                                                                                       
    //backface test                                                                    
    for (size_t i = 0,
        end = triangles.indices.size() / 3;
        i < end; i++)
    {
        const Vec3f& v0 = triangles.vertices[triangles.indices[i * 3]];
        const Vec3f& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
        const Vec3f& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];
        Vec3f normal = (v2 - v1).cross(v0 - v1);
        triangles.backFace[i] = Vec3f(0.0f, 0.0f, 1.0f).dot(normal) >= 0.0f;
    }
    
    //draw triangle
    for (size_t i = 0,
        end = triangles.indices.size() / 3;
        i < end; i++)
    {
        if (!triangles.backFace[i])
        {
            ScanLineTriangle(
                triangles.vertices[triangles.indices[i*3]], 
                triangles.vertices[triangles.indices[i * 3 + 1]], 
                triangles.vertices[triangles.indices[i * 3 + 2]], 
                colors[i] 
            );
        }
        
    }
}

//void drawModel()
//{
//    ConvertToScreen screen;
//    Model model;
//    float zvp = 5.0f;
//    Vec3f reference = { 0.0f, 0.0f, -5.0f };
//    //auto lines = cube.GetLines();
//    auto triangles = model.GetTriangle();
//    //Mat4f scale = Mat4f::Scaling(10.0f);
//    Mat4f rotation =
//        Mat4f::RotationX(pitchX) *
//        Mat4f::RotationY(yawY) *
//        Mat4f::RotationZ(rollZ);
//    Mat4f translate = Mat4f::Translation(0.0f, 0.0f, offsetZ);
//    //Mat4f perspective = Mat4f::PerspectiveFOV(120, WIDTH/HEIGHT, 0.7, -0.5);
//    Mat4f composite = translate * rotation;
//    //Transform from model space to view space
//    for (auto& v : triangles.vertices)
//    {
//        v = v.Normalize(v);
//        v = composite * v;
//        //v = v + Vec3f(0.0f, 0.0f, offsetZ);
//        screen.Transform(v);
//        
//        PerspectiveView(zvp, reference, v);
//    }
//    //backface test                                                                    
//    for (size_t i = 0,
//        end = triangles.vertices.size() / 3;
//        i < end; i++)
//    {
//        const Vec3f& v0 = triangles.vertices[i];
//        const Vec3f& v1 = triangles.vertices[i + 1];
//        const Vec3f& v2 = triangles.vertices[i + 2];
//        Vec3f normal = (v2 - v1).cross(v0 - v1);
//        triangles.backFace[i] = Vec3f(0.0f, 0.0f, 1.0f).dot(normal) >= 0.0f;
//    }
//    size_t count = triangles.vertices.size() / 3;
//    //draw triangle
//    std::cout << triangles.vertices[0].x << " " << triangles.vertices[0].y << " " << triangles.vertices[0].z << std::endl;
//    for (size_t i = 0,
//        end = triangles.vertices.size() / 3;
//        i < end; i++)
//    {
//        /*Vec3f point1(triangles.vertices[*i].x, lines.vertices[*i].y, 0);
//        Vec3f point2(lines.vertices[*(i+1)].x, lines.vertices[*(i + 1)].y, 0);
//        Bressenham(point1.x, point1.y, point2.x, point2.y, WHITE);*/
//        if (!triangles.backFace[i])
//        {
//            ScanLineTriangle(
//                triangles.vertices[i],
//                triangles.vertices[i + 1],
//                triangles.vertices[i + 2],
//                GREEN
//            );
//        }
//    }
//}

void keyboardRot(unsigned char key, int x, int y)
{
    const float dt = 1.0f / 60.0f;
    switch (key) {
    case 'q':
        pitchX += delta * dt;
        break;
    case 'w':
        yawY += delta * dt;
        break;
    case 'e':
        rollZ += delta * dt;
        break;
    case 'a':
        pitchX -= delta * dt;
        break;
    case 's':
        yawY -= delta * dt;
        break;
    case 'd':
        rollZ -= delta * dt;
        break;
    case 'r':
        offsetZ += 3.0f * dt;
        //s += 1.0f;
        break;
    case 'f':
        offsetZ -= 3.0f * dt;
        //s -= 1.0f;
        break;
    case 8:     //Backspace key
        pitchX = yawY = rollZ = 0.0f;
        offsetZ = 30.0f;
        break;
    case 27:    //ESC key
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();  //Window redraw
}

void drawLine()
{
    Vec3f point1(200, 200, 0);
    Vec3f point2(-200, -200, 0);
    Vec3f point3(300, 250, 0);
    Bressenham(point1.x, point1.y, point2.x, point2.y, GREEN);
    Bressenham(point2.x, point2.y, point3.x, point3.y, RED);
    Bressenham(point3.x, point3.y, point1.x, point1.y, YELLOW);
    Mat4f rotX;
    Mat4f trans;
    Mat4f composite;
    rotX = rotX.RotationX(320);
    trans = trans.Translation(200,150,100);
    composite = rotX * trans;
    point1 = composite * point1;
    point2 = composite * point2;
    point3 = composite * point3;
    Bressenham(point1.x, point1.y, point2.x, point2.y, GREEN);
    Bressenham(point2.x, point2.y, point3.x, point3.y, RED);
    Bressenham(point3.x, point3.y, point1.x, point1.y, YELLOW);
}

void display()
{
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    //glLoadIdentity();

    drawCube();
    //drawModel();

    glFlush();
    glutSwapBuffers();

}

int main(int argc, char* argv[]) {

    //  Initialize GLUT and process user parameters
    glutInit(&argc, argv);

    //  Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(WIDTH, HEIGHT);
    //glutInitWindowPosition(0, 0);

    
    // Create window
    glutCreateWindow("Awesome Cube");
    



    //  Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);

    // Callback functions
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardRot);
    //glutIdleFunc(drawLine);
    myinit();
    //  Pass control to GLUT for events
    glutMainLoop();

    //  Return to OS
    return 0;

}