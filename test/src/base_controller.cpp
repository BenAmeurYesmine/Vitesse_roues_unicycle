#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <std_msgs/Int32.h>
#include "std_msgs/Float32.h"

using namespace std;
float rayon_robot; //Le rayon du robot ainsi que le rayon de la roue sont intégré dans le launch file pour que le noeud soit réutilisable
float rayon_roue;
float vd = 0.0; // vitesse roue droite variable globale
float vg = 0.0;   // vitesse roue gauche variable globale
ros::Time last_time;

ros::Publisher v_droite;
ros::Publisher v_gauche;

/**
 * @brief cette fonction retourne la vitesse angulaire de chaque roue du robot
 * 
 * @param twist_aux est la vitesse du robot 
 */

void vitesseCallback(const geometry_msgs::Twist &twist_aux) {
  geometry_msgs::Twist twist = twist_aux;
  float vitesse_x = twist_aux.linear.x;   // vitesse lineaire v
  float vitesse_th = twist_aux.angular.z; // vitesse angulaire theta
  float vitesse_droite = 0;               // vitesse roue droite
  float vitesse_gauche = 0;               // vitesse roue gauche

  vitesse_droite = (vitesse_x + vitesse_th * rayon_robot) / rayon_roue;
  vitesse_gauche = (vitesse_x - vitesse_th * rayon_robot) / rayon_roue;
 
  std_msgs::Float32 vd_;
  vd_.data=vitesse_droite;
  v_droite.publish(vd_);

  std_msgs::Float32 vg_;
  vg_.data=vitesse_gauche;
  v_gauche.publish(vg_);
  }



int main(int argc, char** argv){
  ros::init(argc, argv, "base_controller");
  ros::NodeHandle nh;
  
  
  nh.getParam("/base_controller/rayon_robot", rayon_robot);
  nh.getParam("/base_controller/rayon_roue", rayon_roue);
  
  ros::Rate loop_rate(20);

  ros::Subscriber calcul_v = nh.subscribe("cmd_vel", 5,vitesseCallback) ;
  v_droite=nh.advertise<std_msgs::Float32>("vitesse/droite",5);
  v_gauche=nh.advertise<std_msgs::Float32>("vitesse/gauche",5);
 

  ros::spin();
  return 0;
}
