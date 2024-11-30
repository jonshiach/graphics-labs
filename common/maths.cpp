#include <common/maths.hpp>

float Maths::radians(float angle)
{
    return angle * M_PI / 180.0f;
}

glm::mat4 Maths::scale(const glm::vec3 &v)
{
    glm::mat4 scale(1.0f);
    scale[0][0]  = v.x; scale[1][1]  = v.y; scale[2][2] = v.z;
    return scale;
}

glm::mat4 Maths::translate(const glm::vec3 &v)
{
    glm::mat4 translate(1.0f);
    translate[3][0] = v.x, translate[3][1] = v.y, translate[3][2] = v.z;
    return translate;
}

glm::mat4 Maths::rotate(const float &angle, glm::vec3 v)
{
    v = glm::normalize(v);
    float c = cos(angle);
    float s = sin(angle);
    float x2 = v.x * v.x, y2 = v.y * v.y, z2 = v.z * v.z;
    float xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
    float xs = v.x * s,   ys = v.y * s,   zs = v.z * s;
    
    glm::mat4 rotate;
    rotate[0][0] = (1 - c) * x2 + c;
    rotate[0][1] = (1 - c) * xy + zs;
    rotate[0][2] = (1 - c) * xz - ys;
    rotate[1][0] = (1 - c) * xy - zs;
    rotate[1][1] = (1 - c) * y2 + c;
    rotate[1][2] = (1 - c) * yz + xs;
    rotate[2][0] = (1 - c) * xz + ys;
    rotate[2][1] = (1 - c) * yz - xs;
    rotate[2][2] = (1 - c) * z2 + c;
    
    return rotate;
}

glm::mat4 Maths::lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 worldUp)
{
    glm::vec3 front = glm::normalize(target - eye);
    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    glm::vec3 up    = glm::cross(right, front);
    
    glm::mat4 view;
    view[0][0] = right.x, view[0][1] = up.x, view[0][2] = -front.x;
    view[1][0] = right.y, view[1][1] = up.y, view[1][2] = -front.y;
    view[2][0] = right.z, view[2][1] = up.z, view[2][2] = -front.z;
    view[3][0] = -glm::dot(eye, right);
    view[3][1] = -glm::dot(eye, up);
    view[3][2] =  glm::dot(eye, front);

    return view;
}

glm::mat4 Maths::orthographic(const float left,   const float right,
                              const float bottom, const float top,
                              const float near,   const float far)
{
    glm::mat4 projection;
    projection[0][0] = 2.0f / (right - left);
    projection[1][1] = 2.0f / (top - bottom);
    projection[2][2] = 2.0f / (near - far);
    projection[3][0] = -(right + left) / (right - left);
    projection[3][1] = -(top + bottom) / (top - bottom);
    projection[3][2] =  (near + far) / (near - far);
    
    return projection;
}

glm::mat4 Maths::perspective(const float fov,  const float aspect,
                             const float near, const float far)
{
    float top   = near * tan(fov / 2.0f);
    float right = aspect * top;

    glm::mat4 projection;
    projection[0][0] = near / right;
    projection[1][1] = near / top;
    projection[2][2] = -(far + near) / (far - near);
    projection[2][3] = -1.0f;
    projection[3][2] = -2.0f * far * near / (far - near);
    
    return projection;
}

// =============================================================================
// Quaternion class methods

//Quaternion::Quaternion() {}
//
//Quaternion::Quaternion(const float w, const float x, const float y,
//                       const float z)
//{
//    this->w = w;
//    this->x = x;
//    this->y = y;
//    this->z = z;
//}
//
//glm::mat4 Quaternion::matrix()
//{
//    float x2 = 2.0f * x,  y2 = 2.0f * y,  z2 = 2.0f * z;
//    float xx = x * x2, xy = x * y2, xz = x * z2, xw = w * x2;
//    float yy = y * y2, yz = y * z2, yw = w * y2;
//    float zz = z * z2, zw = w * z2;
//    
//    glm::mat4 R(1.0f);
//    R[0][0] = 1.0f - yy - zz, R[] = xy - zw,        R[8]  = xz + yw;
//    R[1] = xy + zw,        R[5] = 1.0f - xx - zz, R[9]  = yz - xw;
//    R[2] = xz - yw,        R[6] = xw + yz,        R[10] = 1.0f - xx - yy;
//    
//    return R;
//}
//
//void Quaternion::eulerToQuat(const float yaw, const float pitch,
//                             const float roll)
//{
//    float cp = cos(0.5f * pitch), sp = sin(0.5f * pitch);
//    float cy = cos(0.5f * yaw),   sy = sin(0.5f * yaw);
//    float cr = cos(0.5f * roll),  sr = sin(0.5f * roll);
//
////    w = cy * cp * cr - cy * sp * sr;
////    x = cy * cp * cr + cy * sp * sr;
////    y = cy * cp * sp - cy * cp * sr;
////    z = cy * cp * sr + cy * sp * cr;
//    
//    w = cr * cp * cy + sr * sp * sy;
//    x = sr * cp * cy - cr * sp * sy;
//    y = cr * sp * cy + sr * cp * sy;
//    z = cr * cp * sy - sr * sp * cy;
//}
