#include <vector>
#include "Vec3.h"
#include "Mat4.h"
#include "Light.h"

#define PI 3.14159265359

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    Vec3f CameraPos;
    Vec3f CameraFront;
    Vec3f Up;
    Vec3f CameraRight;
    Vec3f CameraDirection;
    Vec3f WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(Vec3f position = Vec3f(0.0f, 0.0f, 0.0f), Vec3f up = Vec3f(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : CameraFront(Vec3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        CameraPos = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
   
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    Mat4f GetViewMatrix()
    {
        Vec3f camDir = CameraPos - (CameraPos + CameraFront);
        Vec3f Camdirs = Normalized(camDir);
        CameraDirection = Camdirs;
        Vec3f camRight = WorldUp.cross(CameraDirection);
        Vec3f Camrig = Normalized(camRight);
        CameraRight = Camrig;
        Up = CameraDirection.cross(CameraRight);
        Mat4f lookAt = Mat4f::RotView(CameraRight, Up, CameraDirection) * Mat4f::TranslateView(CameraPos);
        return lookAt;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            CameraPos += CameraFront * velocity;
        if (direction == BACKWARD)
            CameraPos -= CameraFront * velocity;
        if (direction == LEFT)
            CameraPos -= CameraRight * velocity;
        if (direction == RIGHT)
            CameraPos += CameraRight * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        Vec3f front;
        front.x = cos(Yaw * (PI / 180)) * cos(Pitch * (PI / 180));
        front.y = sin(Pitch * (PI / 180));
        front.z = sin(Yaw * (PI / 180)) * cos(Pitch * (PI / 180));
        Vec3f camFront = Normalized(front);
        CameraFront = camFront;
        // also re-calculate the Right and Up vector
        Vec3f right = CameraFront.cross(WorldUp);
        Vec3f camRig = Normalized(right);  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        CameraRight = camRig;
        Vec3f camUp = CameraRight.cross(CameraFront);
        Vec3f ups = Normalized(camUp);
        Up = ups;
    }
};
