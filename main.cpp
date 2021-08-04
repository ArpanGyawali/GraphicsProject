#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include "Line.h"
#include "Vec3.h"
#include "Mat4.h"
//#include "Cube.h"
#include "ConvertToScreen.h"
//#include "DrawTriangle.h"
#include "DrawInterpolatedTriangle.h"
#include "Color.h"
#include "Light.h"
#include "Projection.h"
#include "Vertex.h"
#include "Camera.h"
//#include "TextureLoader.h"
#include "Model.h"

#define WIDTH 1000
#define HEIGHT 1000

static constexpr float delta = PI;
//float s = 30.0f;
float offsetZ = 2.0f;
float pitchX = 0.0f;
float yawY = 0.0f;
float rollZ = 0.0f;
float lightX = 0.0f;
float lightY = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Camera camera(Vec3f(0.0f, 0.0f, 1.0f));

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


//void drawCube()
//{
//    ConvertToScreen screen;
//    //DrawTri interpolatedTri;
//    Cube cube(0.4);
//    Vec3f view = Vec3f(0.0f, 0.0f, 1.0f);
//    Vec3f reference = Vec3f(0.0f, 0.0f, -1.0f);
//    Vec3f Ka = Vec3f(1.0f, 1.0f, 1.0f);
//    Vec3f Kd = Vec3f(0.8f, 0.8f, 0.8f);
//    Vec3f Ks = Vec3f(0.5f, 0.5f, 0.5f);
//    Vec3f light = Vec3f(lightX, lightY, 5.0f);
//    float ns = 5.0f;
//    Vec3f Ia = Vec3f(0.2f, 0.2f, 0.2f);
//    Vec3f Il = Vec3f(0.7f, 0.7f, 0.7f);
//
//    //auto lines = cube.GetLines();
//    auto triangles = cube.GetTriangles();
//    //Mat4f scale = Mat4f::Scaling(s);
//    Mat4f rotation =
//        Mat4f::RotationX(pitchX) *
//        Mat4f::RotationY(yawY) *
//        Mat4f::RotationZ(rollZ);
//    Mat4f translate = Mat4f::Translation(0.0f, 0.0f, offsetZ);
//    //Mat4f perspective = Mat4f::PerspectiveFOV(120, WIDTH/HEIGHT, 0.7, -0.5);
//    Mat4f composite = translate * rotation;
//    //Transform from model space to view space
//    for (auto& n : triangles.normals)
//    {
//        n = rotation * n;
//    }
//    for (auto& v : triangles.vertices)
//    {
//        v = composite * v;
//        //v = v + Vec3f(0.0f, 0.0f, offsetZ);
//        screen.Transform(v);
//        PerspectiveView(view.z, reference, v);
//    }                                                                                       
//    //backface test                                                                    
//    for (size_t i = 0,
//        end = triangles.indices.size() / 3;
//        i < end; i++)
//    {
//        const Vec3f& v0 = triangles.vertices[triangles.indices[i * 3]];
//        const Vec3f& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
//        const Vec3f& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];
//        Vec3f normal = (v2 - v1).cross(v0 - v1);
//        triangles.backFace[i] = view.dot(normal) >= 0.0f;
//    }
//    
//    //draw triangle
//    for (size_t i = 0,
//        end = triangles.indices.size() / 3;
//        i < end; i++)
//    {
//        Vec3f v01 = triangles.vertices[triangles.indices[i * 3]];
//        Vec3f v02 = triangles.vertices[triangles.indices[i * 3 + 1]];
//        Vec3f v03 = triangles.vertices[triangles.indices[i * 3 + 2]];
//        Vec3f n1 = triangles.normals[triangles.indices[i * 3]];
//        Vec3f n2 = triangles.normals[triangles.indices[i * 3 + 1]];
//        Vec3f n3 = triangles.normals[triangles.indices[i * 3 + 2]];
//        /*Vec3f t1 = triangles.tc[triangles.indices[i * 3]];
//        Vec3f t2 = triangles.tc[triangles.indices[i * 3 + 1]];
//        Vec3f t3 = triangles.tc[triangles.indices[i * 3 + 2]];*/
//        Vec3f vc1 = calculateIntensity(Ka, Kd, Ks, ns, v01, light, view, n1, Ia, Il);
//        Vec3f vc2 = calculateIntensity(Ka, Kd, Ks, ns, v02, light, view, n2, Ia, Il);
//        Vec3f vc3 = calculateIntensity(Ka, Kd, Ks, ns, v03, light, view, n3, Ia, Il);
//        Vec3f face = (vc1 + vc2 + vc3) / 3.0f;
//        Color intensityColor = { face.x, face.y, face.z };
//        Vertex v1(v01, vc1);
//        Vertex v2(v02, vc2);
//        Vertex v3(v03, vc3);
//        /*Vertex v1(v01, t1);
//        Vertex v2(v02, t2);
//        Vertex v3(v03, t3); */
//        if (!triangles.backFace[i])
//        {
//            ProcessTriangle(v1, v2, v3);
//            /*ScanLineTriangle(
//                triangles.vertices[triangles.indices[i*3]], 
//                triangles.vertices[triangles.indices[i * 3 + 1]], 
//                triangles.vertices[triangles.indices[i * 3 + 2]], 
//                intensityColor
//            );*/
//        }
//        
//    }
//}

void drawModel()
{
    ConvertToScreen screen;
    Model model;
    //Vec3f view = Vec3f(0.0f, 0.0f, 1.0f);
    Vec3f reference = Vec3f(0.0f, 0.0f, 0.0f);
    Vec3f Ka = Vec3f(1.0f, 1.0f, 1.0f);
    Vec3f Kd = Vec3f(0.8f, 0.8f, 0.8f);
    Vec3f Ks = Vec3f(0.8f, 0.8f, 0.8f);
    Vec3f light = Vec3f(lightX, lightY, 5.0f);
    float ns = 255.0f;
    Vec3f Ia = Vec3f(0.2f, 0.2f, 0.2f);
    Vec3f Il = Vec3f(0.7f, 0.7f, 0.7f);
    //auto lines = cube.GetLines();
    auto triangles = model.GetTriangle();
    //Mat4f scale = Mat4f::Scaling(10.0f);
    Mat4f rotation =
        Mat4f::RotationX(pitchX) *
        Mat4f::RotationY(yawY) *
        Mat4f::RotationZ(rollZ);
    Mat4f translate = Mat4f::Translation(0.0f, 0.0f, offsetZ);
    Mat4f view = camera.GetViewMatrix();
    //Mat4f perspective = Mat4f::PerspectiveFOV(120, WIDTH/HEIGHT, 0.7, -0.5);
    Mat4f composite = translate * rotation;
    /*for (auto& n : triangles.normals)
        {
            n = rotation * n;
        }*/
    //Transform from model space to view space
    for (auto& v : triangles.vertices)
    {
        v = composite * v;          //modeling transformation(MC -> WC)
        //Viewing Transfrmaion(WC -> VC)
        v = view * v;
        /*Mat4f perspective = PerspectiveFOV((camera.Zoom * (PI / 180)), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        v = perspective * v;*/
        PerspectiveView(camera.CameraPos.z, reference, v);  //projection transformation(VC -> PC)
        auto norm = Normalized(v);              //PC -> NDC
        v = screen.Transform(norm);             //Workstation transformation(NDC -> DC)
    }
    //backface test                                                                    
    for (size_t i = 0,
        end = triangles.vertices.size() / 3;
        i < end; i=i+3)
    {
        const Vec3f& v0 = triangles.vertices[i];
        const Vec3f& v1 = triangles.vertices[i + 1];
        const Vec3f& v2 = triangles.vertices[i + 2];
        Vec3f normal = (v1 - v2).cross(v0 - v2);
        //triangles.backFace[i] = view.dot(normal) >= 0.0f;
    }
    size_t count = triangles.vertices.size() / 3;
    //draw triangle
    std::cout << triangles.vertices[0].x << " " << triangles.vertices[0].y << " " << triangles.vertices[0].z << std::endl;
    for (size_t i = 0,
        end = triangles.vertices.size() / 3;
        i < end; i=i+3)
    {
        Vec3f v01 = triangles.vertices[i];
        Vec3f v02 = triangles.vertices[i + 1];
        Vec3f v03 = triangles.vertices[i + 2];
        Vec3f n1 = triangles.normals[i];
        Vec3f n2 = triangles.normals[i + 1];
        Vec3f n3 = triangles.normals[i + 2];
        Vec3f vc1 = calculateIntensity(Ka, Kd, Ks, ns, v01, light, camera.CameraPos, n1, Ia, Il);
        Vec3f vc2 = calculateIntensity(Ka, Kd, Ks, ns, v02, light, camera.CameraPos, n2, Ia, Il);
        Vec3f vc3 = calculateIntensity(Ka, Kd, Ks, ns, v03, light, camera.CameraPos, n3, Ia, Il);
        Vertex v1(v01, n1, vc1);
        Vertex v2(v02, n2, vc2);
        Vertex v3(v03, n3, vc3);
        
            ProcessTriangle(v1, v2, v3);
        
    }
}

//void keyboard(unsigned char key, int x, int y)
//{
//    const float dt = 1.0f / 60.0f;
//    if (lightX > 200.0f)
//        lightX = 0.0f;
//    if (lightY > 200.0f)
//        lightY = 0.0f;
//    switch (key) {
//    case 'q':
//        pitchX += delta * dt;
//        break;
//    case 'w':
//        yawY += delta * dt;
//        break;
//    case 'e':
//        rollZ += delta * dt;
//        break;
//    case 'a':
//        pitchX -= delta * dt;
//        break;
//    case 's':
//        yawY -= delta * dt;
//        break;
//    case 'd':
//        rollZ -= delta * dt;
//        break;
//    case 'r':
//        offsetZ += 3.0f * dt;
//        //s += 1.0f;
//        break;
//    case 'f':
//        offsetZ -= 3.0f * dt;
//        //s -= 1.0f;
//        break;
//    case 't':
//        lightX += 5.0f;
//        //s += 1.0f;
//        break;
//    case 'g':
//        lightY += 5.0f;
//        //s -= 1.0f;
//        break;
//    case 8:     //Backspace key
//        pitchX = yawY = rollZ = 0.0f;
//        lightX = lightY = 0.0f;
//        offsetZ = 2.0f;
//        break;
//    case 27:    //ESC key
//        exit(0);
//        break;
//    default:
//        break;
//    }
//    glutPostRedisplay();  //Window redraw
//}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'w':
        camera.ProcessKeyboard(FORWARD, deltaTime);
        break;
    case 's':
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        break;
    case 'a':
        camera.ProcessKeyboard(LEFT, deltaTime);
        break;
    case 'd':
        camera.ProcessKeyboard(RIGHT, deltaTime);
        break;
    case 't':
        lightX += 5.0f;
        //s += 1.0f;
        break;
    case 'g':
        lightY += 5.0f;
        //s -= 1.0f;
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

    //drawCube();
    drawModel();

    glFlush();
    glutSwapBuffers();

}

int main(int argc, char* argv[]) {
    /*Vec3f texture = GetPixel(99, 99);
    std::cout << texture.x << texture.y << texture.z << getWidth() << getHeight() << std::endl;*/

    //  Initialize GLUT and process user parameters
    glutInit(&argc, argv);
    float currentFrame = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
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
    glutKeyboardFunc(keyboard);
    //glutIdleFunc(drawLine);
    myinit();
    //  Pass control to GLUT for events
    glutMainLoop();

    //  Return to OS
    return 0;

}