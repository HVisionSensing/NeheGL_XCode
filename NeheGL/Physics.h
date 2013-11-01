//
//  Physics.h
//  NeheGL
//
//  Created by Andong Li on 10/27/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef NeheGL_Physics_h
#define NeheGL_Physics_h

#include <math.h>

class Vector3D_PHY{
public:
	float x;
	float y;
	float z;
	
	Vector3D_PHY(){
		x = 0;
		y = 0;
		z = 0;
	}
	
	Vector3D_PHY(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	Vector3D_PHY& operator= (Vector3D_PHY v){
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	
	Vector3D_PHY operator+ (Vector3D_PHY v){
		return Vector3D_PHY(x + v.x, y + v.y, z + v.z);
	}
	
	Vector3D_PHY operator- (Vector3D_PHY v){
		return Vector3D_PHY(x - v.x, y - v.y, z - v.z);
	}
	
	Vector3D_PHY operator* (float value){
		return Vector3D_PHY(x * value, y * value, z * value);
	}
	
	Vector3D_PHY operator/ (float value){
		return Vector3D_PHY(x / value, y / value, z / value);
	}
	
	Vector3D_PHY& operator+= (Vector3D_PHY v){
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	Vector3D_PHY& operator-= (Vector3D_PHY v){
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	
	Vector3D_PHY& operator*= (float value){
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}
	
	Vector3D_PHY& operator/= (float value){
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}
	
	
	Vector3D_PHY operator- (){
		return Vector3D_PHY(-x, -y, -z);
	}
	
	float length(){
		return sqrtf(x*x + y*y + z*z);
	};
	
	void unitize(){
		float length = this->length();
		
		if (length == 0)
			return;
		
		x /= length;
		y /= length;
		z /= length;
	}
	
	Vector3D_PHY unit(){
		float length = this->length();
		
		if (length == 0)
			return *this;
		
		return Vector3D_PHY(x / length, y / length, z / length);
	}
	
};


class Mass_PHY{
public:
	float m;
	Vector3D_PHY pos;
	Vector3D_PHY vel;
	Vector3D_PHY force;
	
	Mass_PHY(float m){
		this->m = m;
	}
	
	void applyForce(Vector3D_PHY force){
		this->force += force;
	}
	
	
	void init(){
		force.x = 0;
		force.y = 0;
		force.z = 0;
	}
	
	
	void simulate(float dt){
		vel += (force / m) * dt;
		
		pos += vel * dt;
	}
	
};


class Simulation_PHY{
public:
	int numOfMasses;
	Mass_PHY** masses;
	
	Simulation_PHY(int numOfMasses, float m){
		this->numOfMasses = numOfMasses;
		
		masses = new Mass_PHY*[numOfMasses];
		
		for (int a = 0; a < numOfMasses; ++a)
			masses[a] = new Mass_PHY(m);
	}
	
	virtual void release(){
		for (int a = 0; a < numOfMasses; ++a){
			delete(masses[a]);
			masses[a] = NULL;
		}
		
		delete(masses);
		masses = NULL;
	}
	
	Mass_PHY* getMass(int index){
		if (index < 0 || index >= numOfMasses)
			return NULL;
		
		return masses[index];
	}
	
	virtual void init(){
		for (int a = 0; a < numOfMasses; ++a)
			masses[a]->init();
	}
	
	virtual void solve(){
		
	}
	
	virtual void simulate(float dt){
		for (int a = 0; a < numOfMasses; ++a)
			masses[a]->simulate(dt);
	}
	
	virtual void operate(float dt){
		init();
		solve();
		simulate(dt);
	}
	
};

/*
 class ConstantVelocity is derived from class Simulation
 It creates 1 mass with mass value 1 kg and sets its velocity to (1.0f, 0.0f, 0.0f)
 so that the mass moves in the x direction with 1 m/s velocity.
 */
class ConstantVelocity : public Simulation_PHY{
public:
	//Constructor firstly constructs its super class with 1 mass and 1 kg
	ConstantVelocity() : Simulation_PHY(1, 1.0f){
		masses[0]->pos = Vector3D_PHY(0.0f, 0.0f, 0.0f);
		masses[0]->vel = Vector3D_PHY(1.0f, 0.0f, 0.0f);
	}
	
};


/*
 class MotionUnderGravitation is derived from class Simulation
 It creates 1 mass with mass value 1 kg and sets its velocity to (10.0f, 15.0f, 0.0f) and its position to
 (-10.0f, 0.0f, 0.0f). The purpose of this application is to apply a gravitational force to the mass and
 observe the path it follows. The above velocity and position provides a fine projectile path with a
 9.81 m/s/s downward gravitational acceleration. 9.81 m/s/s is a very close value to the gravitational
 acceleration we experience on the earth.
 */
class MotionUnderGravitation : public Simulation_PHY{
public:
	Vector3D_PHY gravitation;
	
	MotionUnderGravitation(Vector3D_PHY gravitation) : Simulation_PHY(1, 1.0f){
		this->gravitation = gravitation;
		masses[0]->pos = Vector3D_PHY(-10.0f, 0.0f, 0.0f);
		masses[0]->vel = Vector3D_PHY(10.0f, 15.0f, 0.0f);
	}
	
	virtual void solve(){
		for (int a = 0; a < numOfMasses; ++a)
			masses[a]->applyForce(gravitation * masses[a]->m);
	}
	
};

/*
 class MassConnectedWithSpring is derived from class Simulation
 It creates 1 mass with mass value 1 kg and binds the mass to an arbitrary constant point with a spring.
 This point is refered as the connectionPos and the spring has a springConstant value to represent its
 stiffness.
 */
class MassConnectedWithSpring : public Simulation_PHY{
public:
	float springConstant;
	Vector3D_PHY connectionPos;
	
	MassConnectedWithSpring(float springConstant) : Simulation_PHY(1, 1.0f){
		this->springConstant = springConstant;
		
		connectionPos = Vector3D_PHY(0.0f, -5.0f, 0.0f);
		masses[0]->pos = connectionPos + Vector3D_PHY(10.0f, 0.0f, 0.0f);
		masses[0]->vel = Vector3D_PHY(0.0f, 0.0f, 0.0f);
	}
	
	virtual void solve(){
		for (int a = 0; a < numOfMasses; ++a){
			Vector3D_PHY springVector = masses[a]->pos - connectionPos;
			masses[a]->applyForce(-springVector * springConstant);
		}
	}
	
};


#endif
