#include<iostream>
#include"rtweekend.h"
#include"color.h"
#include"sphere.h"
#include"camera.h"
#include"hittable_list.h"
#include"material.h"
#include"bvh_node.h"
#include"rect.h"
#include"light.h"
#include "box.h"
#include "rotate.h"
#include "translate.h"
#include "flip_face.h"
#include "constant_mediun.h"
#include<GL/glfw3.h>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui_c.h>
#include<ctime>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using std::cin;
using std::cout;

const vec3 background(0, 0, 0);


bvh_node simple_light() {
	hittable_list objects;

	auto pertext = make_shared<noise_texture>(4);
	objects.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(4, 4, 4)));
	objects.add(make_shared<sphere>(vec3(0, 7, 0), 2, difflight));
	objects.add(make_shared<rect>(3, 20, 1, 1, -2,5, difflight));

	return  bvh_node(objects, 0, objects.objects.size(), 0.0f, 1.0f);


}

bvh_node cornell_box() {
	hittable_list objects;
	auto red = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.65, 0.05, 0.05)));
	auto white = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.73, 0.73, 0.73)));
	auto green = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.12, 0.45, 0.15)));
	auto light = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(15, 15, 15)));

	objects.add(make_shared<rect>(555, 555, 0, 555, 0, 555, green)); //左边绿色墙壁
	objects.add(make_shared<rect>(0, 0, 0, 555, 0, 555, red));  //右边红色墙壁
	objects.add(make_shared<rect>(213, 343, 554,554, 227, 332, light)); //最上方矩形光源
	objects.add(make_shared<rect>(0, 555, 0, 0, 0, 555, white));   //地面
	objects.add(make_shared<rect>(0, 555, 0, 555, 555, 555, white)); //后方墙壁
	objects.add(make_shared<rect>(0, 555, 555,555, 0, 555, white)); //天花板

	/*objects.add(make_shared<box>(vec3(130, 0, 65), vec3(295, 165, 230), white));
	objects.add(make_shared<box>(vec3(265, 0, 295), vec3(430, 330, 460), white));*/

	shared_ptr<hittable> box1 = make_shared<box>(vec3(0, 0, 0), vec3(165, 330, 165), white);
	box1 = make_shared<rotate>(box1, 15, 1);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	objects.add(box1);

	shared_ptr<hittable> box2 = make_shared<box>(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = make_shared<rotate>(box2, -45, 1);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	objects.add(box2); 

	return  bvh_node(objects, 0, objects.objects.size(), 0.0f, 1.0f);
}
bvh_node cornell_smoke() {
	hittable_list objects;
	auto red = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.65, 0.05, 0.05)));
	auto white = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.73, 0.73, 0.73)));
	auto green = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.12, 0.45, 0.15)));
	auto light = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(15, 15, 15)));

	objects.add(make_shared<rect>(555, 555, 0, 555, 0, 555, green)); //左边绿色墙壁
	objects.add(make_shared<rect>(0, 0, 0, 555, 0, 555, red));  //右边红色墙壁
	objects.add(make_shared<rect>(213, 343, 554, 554, 227, 332, light)); //最上方矩形光源
	objects.add(make_shared<rect>(0, 555, 0, 0, 0, 555, white));   //地面
	objects.add(make_shared<rect>(0, 555, 0, 555, 555, 555, white)); //后方墙壁
	objects.add(make_shared<rect>(0, 555, 555, 555, 0, 555, white)); //天花板


	shared_ptr<hittable> box1 = make_shared<box>(vec3(0, 0, 0), vec3(165, 330, 165), green);
	box1 = make_shared<rotate>(box1, 15, 1);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	

	shared_ptr<hittable> box2 = make_shared<box>(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = make_shared<rotate>(box2, -45, 1);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));

	objects.add(
		make_shared<constant_medium>(box1, 0.01, make_shared<constant_texture>(vec3(0, 0, 0))));
	objects.add(
		make_shared<constant_medium>(box2, 0.01, make_shared<constant_texture>(vec3(1, 1, 1))));
	return  bvh_node(objects, 0, objects.objects.size(), 0.0f, 1.0f);
}

bvh_node two_sphere() {
	hittable_list world;
	auto checker = make_shared<checker_texture>(make_shared<constant_texture>(color(0.2, 0.3, 0.1)), make_shared<constant_texture>(color(0.9, 0.9, 0.9)));

	world.add(make_shared<sphere>(vec3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	world.add(make_shared<sphere>(vec3(0, 10, 0), 10, make_shared<lambertian>(checker)));

	return  bvh_node(world, 0, world.objects.size(), 0.0f, 1.0f);
}

bvh_node two_perlin_spheres() {
	hittable_list world;
	auto pertext = make_shared<noise_texture>(4);
	world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(vec3(0,2, 0), 2, make_shared<lambertian>(pertext)));
	
	return bvh_node(world, 0, world.objects.size(), 0.0f, 1.0f);


}

bvh_node earth() {
	int nx, ny, nn;
	hittable_list world;
	unsigned char* texture_data = stbi_load("earthmap.jpg", &nx, &ny, &nn,0);

	auto earth_surface = make_shared<lambertian>(make_shared<image_texture>(texture_data, nx, ny));
	auto globe = make_shared<sphere>(vec3(0, 0, 0), 2, earth_surface);
	world.add(globe);

	return bvh_node(world, 0, world.objects.size(), 0.0f, 1.0f);

}

bvh_node final_scene() {
	hittable_list world;
	auto ground = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.48, 0.83, 0.53)));
	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double(1, 101);
			auto z1 = z0 + w;

			world.add(make_shared<box>(vec3(x0, y0, z0), vec3(x1, y1, z1), ground));
		}
	}//地面上若干个长方体

	auto light = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(7, 7, 7)));
	world.add(make_shared<rect>(123, 423, 554,554,147, 412, light));//矩形光源

	auto center1 = vec3(400, 400, 200);
	auto center2 = center1 + vec3(30, 0, 0);
	auto moving_sphere_material =
		make_shared<lambertian>(make_shared<constant_texture>(vec3(0.7, 0.3, 0.1)));
	world.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material)); //动态模糊球

	world.add(make_shared<sphere>(vec3(260, 150, 45), 50, make_shared<dielectric>(1.5))); //电解质材质球体
	world.add(make_shared<sphere>(
		vec3(0, 150, 145), 50, make_shared<metal>(vec3(0.8, 0.8, 0.9), 10.0)
		)); //金属球体

	auto boundary = make_shared<sphere>(vec3(360, 150, 145), 70, make_shared<dielectric>(1.5)); //电解质材质的边界？ 存疑，好像材质并没有什么用，因为参与介质的散射用的是isotropic各向同性散射
	world.add(boundary);
	world.add(make_shared<constant_medium>(
		boundary, 0.2, make_shared<constant_texture>(vec3(0.2, 0.4, 0.9))
		));  //参与介质次的表面散射球体1

	boundary = make_shared<sphere>(vec3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
	world.add(make_shared<constant_medium>(
		boundary, .0001, make_shared<constant_texture>(vec3(1, 1, 1)))); //参与介质次的表面散射球体2 


	int nx, ny, nn;
	auto tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	auto emat = make_shared<lambertian>(make_shared<image_texture>(tex_data, nx, ny));   //纹理贴图球体，地球仪
	world.add(make_shared<sphere>(vec3(400, 200, 400), 100, emat));

	auto pertext = make_shared<noise_texture>(0.1);
	world.add(make_shared<sphere>(vec3(220, 280, 300), 80, make_shared<lambertian>(pertext)));  //使用柏林噪声制作的大理石表面球体


	hittable_list box_sphere;
	auto white = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.73, 0.73, 0.73)));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		box_sphere.add(make_shared<sphere>(vec3::random(0, 165), 10, white));
	}

	shared_ptr<hittable> box_spheres = make_shared<hittable_list>(box_sphere);
	//auto box_spheres = bvh_node(box_sphere, 0, box_sphere.objects.size(), 0.0f, 1.0f);


	box_spheres = make_shared<rotate>(box_spheres, 15, 1); //平移
	box_spheres = make_shared<translate>(box_spheres, vec3(-100, 270, 395));//旋转

	world.add(box_spheres);


	return bvh_node(world, 0, world.objects.size(), 0.0f, 1.0f);


}

bvh_node random_scene() {
	hittable_list world;
	auto checker = make_shared<checker_texture>(make_shared<constant_texture>(color(0.2, 0.3, 0.1)), make_shared<constant_texture>(color(0.9, 0.9, 0.9)));

	auto ground_material = make_shared<lambertian>(checker);//地面

	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	

	//for (int a = -5; a < 1; a++) {
	//	for (int b = -5; b < 1; b++) {
	//		auto choose_mat = random_double(); //随机材质
	//		point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()); //随机中心

	//		if ((center - point3(4, 0.2, 0)).length() > 0.9) {
	//			shared_ptr<material> sphere_material;

	//			if (choose_mat < 0.1) {
	//				// diffuse 漫反射
	//				auto albedo = color::random() * color::random();
	//				sphere_material = make_shared<lambertian>(albedo);
	//				world.add(make_shared<sphere>(center, 0.2, sphere_material));
	//			}
	//			else if (choose_mat < 0.8) {
	//				//动态模糊小球
	//				auto albedo = vec3::random() * vec3::random();
	//				world.add(make_shared<moving_sphere>(center, center + vec3(0, random_double(0, 0.5), 0), 0.0, 1.0, 0.2, make_shared<lambertian>(albedo)));
	//			}
	//			else if (choose_mat < 0.95) {
	//				// metal 金属
	//				auto albedo = color::random(0.5, 1);
	//				auto fuzz = random_double(0, 0.5);
	//				sphere_material = make_shared<metal>(albedo, fuzz);
	//				world.add(make_shared<sphere>(center, 0.2, sphere_material));
	//			}
	//			else {
	//				// glass 玻璃
	//				sphere_material = make_shared<dielectric>(1.5);
	//				world.add(make_shared<sphere>(center, 0.2, sphere_material));
	//			}
	//		}
	//	}
	//}
	auto albedo = vec3::random() * vec3::random();
	auto material1 = make_shared<lambertian>(make_shared<constant_texture>(albedo));
	world.add(make_shared<moving_sphere>(point3(0, 1, 0), point3(0, 1, 0) + vec3(0, random_double(0,0.5), 0), 0.0, 1.0, 1.0, material1));
	
	//auto material1 = make_shared<dielectric>(1.5);
	//world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.4, 0.2, 0.1)));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return bvh_node(world,0,world.objects.size(),0.0f,1.0f);
	/*return world;*/
}

color ray_color(const ray& r,const hittable& world,int depth) {
	hit_record rec;
	if (depth < 0)
		return background;

	if (world.hit(r, 0.001, infinity, rec))
	{	
		//point3 target = rec.p + rec.normal + random_unit_vector(); //随机点
		ray scattered;
		color attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		double rand = random_double();
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return emitted + (attenuation * ray_color(scattered, world, depth - 1));

		return emitted;
	

	/*	point3 target = rec.p + random_in_hemisphere(rec.normal);
		return 0.5 * ray_color(ray(rec.p, unit_vector(target - rec.p)), world, depth - 1);*/
	}


	//vec3 unit_direction = unit_vector(r.direction());
	//double t = 0.5 * (unit_direction.y()+ 1.0);

	//线性插值

	return background;
}


int main() {
	//window
	cv::Mat src(800,1200,CV_8UC3, cv::Scalar(0, 0, 255));

	//image
	const double aspect_ratio = 3.0 / 2.0;
	const int image_width = 1200;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 50;
	clock_t startTime, endTime;
	startTime = clock();//计时开始
	
	//world
	
	auto world = final_scene();
	



	//camera(simple_light 场景之前摄像机参数
	//point3 lookfrom(-50, 2,-5);
	//point3 lookat(0, 0, -1);
	//vec3 vup(0, 1, 0);
	//auto dist_to_focus = 10;
	//auto aperture = 0.0;
	//camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus,0.0,1.0);

	//camera (cornell_box 场景 摄像机参数）
	
	point3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.0;
	auto vfov = 40.0;
	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
	

	const vec3 background(0, 0, 0);
	
	cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	for (int j = image_height - 1; j >= 0; --j) {
		for (int i = image_width - 1; i >= 0; --i)
		{	
			color pixel_color(0, 0, 0);
			for (int k = 0; k < samples_per_pixel; ++k)
			{	
				double u = double(i + random_double()) / (image_width - 1);
				double v = double(j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r,world,20);
			}
			auto scale = 1.0 / samples_per_pixel;
			pixel_color *= scale;
			
			cv::Vec3b color_value(clamp(sqrt(pixel_color[2]),0.0,0.999) *256, clamp(sqrt(pixel_color[1]), 0.0, 0.999) *256, clamp(sqrt(pixel_color[0]), 0.0, 0.999) *256);
			src.ptr<cv::Vec3b>(image_height-j-1)[i] = color_value;
			
		}
	}
	imshow("output", src);
	endTime = clock();//计时结束
	std::cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
	cv::waitKey(0);
	return 0;

}
