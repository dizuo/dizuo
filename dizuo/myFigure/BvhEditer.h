#ifndef BVHEDITER_H_
#define BVHEDITER_H_

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "Htree.h"
using namespace std;

class BvhEditer : public Htree
{
public:
	typedef Joint part;

	BvhEditer();
	BvhEditer(std::string name);
	virtual ~BvhEditer();

	bool process_file();
	bool recurs_read();
	void recurs_write(ofstream &outFile);
public:
	int		 get_frame_num()const{return frames_num;};
	float	 get_frame_time()const{return frame_time;}
	int		 get_channel_num()const{return channel_count;}
	int		 get_node_num()const{return node_count;};

	const part*	get_root()const{return root;}
	part*		get_joint(const std::string &name);
	part*		get_joint(const char* name);
	const std::vector<part*>& get_joints_vector()const {return linear_joint_vec;}
	const std::string get_file_name() const { return file_name; }

	float	get_motion( int frame, int channel ) const { return  motion_data[ frame * channel_count + channel ]; }
	void	set_motion( int frame, int channel, float value ) { motion_data[ frame * channel_count + channel ] = value; }
protected:
	void clear();
													//以joint为单位递归操作
	bool generate_joint(ifstream& inFile, part* node, part* parent);
	void write_joint(ofstream &outFile, part* node, int level);

	part		*root;
	std::string file_name;
	float		frame_time;
	int			frames_num;
	int			node_count;
	int			channel_count;

	std::vector<part*> linear_joint_vec;
	typedef std::vector<part*>::iterator vec_iter;
	std::vector<float> motion_data;
};
//  [4/1/2009 RenYaFei]
//		读取
//		存储
//		增加某一帧数据

#endif