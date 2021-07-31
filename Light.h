#pragma once

#include "Vec3.h"
#include "Color.h"
#include "ConvertToScreen.h"

Vec3f& Normalized(Vec3f& p)
{
	Vec3f normalized;
	float mag = p.x * p.x + p.y * p.y + p.z * p.z;
	mag = pow(mag, 0.5);
	normalized = p.operator/(mag);
	return normalized;
}

Vec3f calculateIntensity(const Vec3f& Ka, const Vec3f& Kd, const Vec3f& Ks, const float ns, Vec3f& point,Vec3f& light, const Vec3f& view, const Vec3f& normal, const Vec3f& Ia, const Vec3f& Il)
{
	const Vec3f ambientColor = Ka * Ia;
	Vec3f lightVec = light - point;
	const Vec3f unitLight = Normalized(lightVec);
	const Vec3f diffuseColor = (Kd * Il * normal.dot(unitLight)).max();
	const Vec3f reflection = (normal * 2.0 * normal.dot(unitLight)) - unitLight;
	const Vec3f specularColor = (Ks * Il * pow(view.dot(reflection), ns)).max();
	const Vec3f intensity = ambientColor + diffuseColor + specularColor;
	return intensity;
}
