#include "BvhEditer.h"

#include <iomanip>

using namespace std;

class file_not_found{};

BvhEditer::BvhEditer()
	: root(0)
{	clear(); }

BvhEditer::BvhEditer(std::string _name)
	: root(NULL)
	, file_name(_name)
{	clear(); }

BvhEditer::~BvhEditer()
{
	Htree::destroy_tree( root ); 
}

void BvhEditer::clear()
{
	frame_time = 0.0;
	frames_num = 0;
	node_count = 0;
	channel_count = 0;

	linear_joint_vec.clear();
	motion_data.clear();
}

//递归实现
bool BvhEditer::recurs_read()
{
	ifstream inFile( file_name.c_str() );
	if ( !inFile ) {
		cout << "File \"" << file_name << "\" not found.\n";
		throw file_not_found(); 
		return false;
	}

	bool returnType = true;

	string readString;
	double readdouble;

	// Reading "HIERARCHY"
	inFile >> readString;
	// Reading in all the roots.
	inFile >> readString;
	while(readString == "ROOT")
	{
		root = new part;
		node_count++;
		linear_joint_vec.push_back( root );

		if( !this->generate_joint(inFile, root, NULL) )
			return false;
		inFile >> readString;
	}

	// Now getting motion data:
	inFile >> readString;
	if(readString != "Frames:")
		returnType = false;
	inFile >> frames_num;

	// Getting sampling rate info:
	inFile >> readString;
	if(readString != "Frame")
		returnType = false;
	inFile >> readString;
	if(readString != "Time:")
		returnType = false;
	inFile >> frame_time; 

	for(int i = 0; i < frames_num*channel_count; i++)
	{
		inFile>>readdouble;
		motion_data.push_back( (float)readdouble );
	}
	
	inFile.close();
	return returnType;
}

//递归实现
bool BvhEditer::generate_joint(ifstream& inFile, part* node, part* parent)
{
	string readString;
	double readdouble;
	char readChar;

	part *nextNode;

	// Read the name of the BodyPart
	inFile >> readString;
	node->name = readString;

	// Next we check to see if we get a '{'
	inFile >> readChar;
	if(readChar != '{') 
		return false;   //error

	// Next we should read "OFFSET" to be the local offset
	inFile >> readString;
	if(readString != "OFFSET")
		return false;    //error

	inFile >> readdouble;    node->offset[0] = readdouble;//X
	inFile >> readdouble;    node->offset[1] = readdouble;//Y
	inFile >> readdouble;    node->offset[2] = readdouble;//Z

	// Updating global offsets:
	// We just add our local offset to the previous global offset.

	// If we do NOT have an End Site.
	if(node->name != "Site")
	{
		// Next we should read "CHANNELS"
		inFile >> readString;
		if(readString != "CHANNELS")
			return false; //error

		// This information goes into the Motion class node.

		// Next we read in the number of channels we have.
		inFile >> readdouble;
		for(int i = 0; i < readdouble; i++)
		{
			inFile >> readString;
			Joint::Channel *chan = new Joint::Channel;
			chan->index = channel_count++;

			if (readString == "Xposition") {		
				chan->type = part::Xpos;
			} else if (readString == "Yposition") {					
				chan->type = part::Ypos;	
			} else if (readString == "Zposition") {				
				chan->type = part::Zpos;	
			} else if (readString == "Zrotation") {
				chan->type = part::Zrot;				
			} else if (readString == "Yrotation") {
				chan->type = part::Yrot;				
			} else if (readString == "Xrotation") {
				chan->type = part::Xrot;
			}
			node->channels.push_back( chan );
		}

		// Now we have a loop so that we continue to create links in
		// the correct hierarchical location:
		inFile >> readString;
		while(readString == "JOINT" || readString == "End")
		{
			nextNode = new part;
			node_count++;
			linear_joint_vec.push_back( nextNode );

			node->children.push_back(nextNode);
			nextNode->parent = node;
			generate_joint(inFile, nextNode, node);
			// Once we finish all of nextNode, we must see if we have
			// any more links.
			inFile >> readString;
		}
	}
	// If we have read "End"...
	else
	{
		//... the next data that has not been read must be '}'
		inFile >> readChar;
		if(readChar != '}')
			return false; //error
	}

	return true;
}

// ----------------------------------------------------------------
//	Name：			BvhEditer::process_file()
//	Description:	利用栈解析BVH文件
//	Return Value:	bool
// ----------------------------------------------------------------
bool BvhEditer::process_file()
{
	ifstream bvh_stream( file_name.c_str() );
	if (!bvh_stream) {
		cout << "File \"" << file_name << "\" not found.\n";
		throw file_not_found(); 
		return false;
	}

	istream_iterator<string> bvh_it(bvh_stream);
	istream_iterator<string> sentinel;

	vector<string> lines(bvh_it,sentinel);

	vector<part*>   nodes_stack;
	part *work_node = 0;
	part *new_node = 0;

	unsigned channel_num = 0;
	//channel_count给Channel里面的index赋值

	//处理所有的数据
	for( size_t i=0; i<lines.size(); i++ )
	{
		if( lines[i]=="{" )
		{
			nodes_stack.push_back( new_node );
			work_node = new_node;
			continue;
		}

		if( lines[i]=="}" )
		{
			if( nodes_stack.back()->name=="Site" )	
			{
				nodes_stack.pop_back();
				continue;
			}
			nodes_stack.pop_back();			
			if( nodes_stack.size() > 0 )		
				//最后一次，根节点的弹出处理
				work_node = nodes_stack.back();

			continue;
		}

		if( lines[i]=="ROOT" || lines[i]=="JOINT" || lines[i]=="End" )
		{
			new_node = new part;
			if( lines[i] == "ROOT" )
				root = new_node;
			new_node->name = lines[++i];	//标志下一个为 name
			linear_joint_vec.push_back( new_node );

			node_count++;

			new_node->parent = work_node;
			if( work_node )
				work_node->children.push_back( new_node );

			continue;
		}

		if( lines[i]=="OFFSET" )
		{
			work_node->offset[0] = atof( lines[++i].c_str() );
			work_node->offset[1] = atof( lines[++i].c_str() );
			work_node->offset[2] = atof( lines[++i].c_str() );

			continue;
		}

		if( lines[i]=="CHANNELS" ) //CHANNELS
		{
			if (channel_num == 0) {
				channel_num = atoi( lines[++i].c_str() );
			} 

			//循环
			for (size_t bg=i+1, end=i+channel_num; bg<=end; bg++)
			{
				Joint::Channel *chan = new Joint::Channel;
				chan->index = channel_count++;

				if (lines[bg] == "Xposition") {		
					chan->type = part::Xpos;
				} else if (lines[bg] == "Yposition") {
					chan->type = part::Ypos;	
				} else if (lines[bg] == "Zposition") {
					chan->type = part::Zpos;	
				} else if (lines[bg] == "Zrotation") {
					chan->type = part::Zrot;
				} else if (lines[bg] == "Yrotation") {
					chan->type = part::Yrot;
				} else if (lines[bg] == "Xrotation") {
					chan->type = part::Xrot;
				}
				work_node->channels.push_back( chan );
			}

			i = i+channel_num;

			if( work_node->channels.size()==channel_num )
				channel_num = 0;

			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		//MOTION
		if( lines[i]=="Frames:" )
			frames_num = atoi( lines[++i].c_str() );

		if( lines[i]=="Time:" )
		{
			frame_time = (float)atof( lines[++i].c_str() );

			//调用recurs_render()将结点存入bvh_nodes_linear中:
			//recurs_render(root, 0);

			//存放数据:
			for(i++; i < lines.size(); i++)
				motion_data.push_back( (float)atof( lines[i].c_str() ) );

			break;
		}
	}

	bvh_stream.close();
	return true;
}

// ----------------------------------------------------------------
//	Name：			BvhEditer::get_joint(const std::string &name)
//	Description:	重载函数
//	Return Value:	joint*
// ----------------------------------------------------------------
BvhEditer::part* BvhEditer::get_joint(const std::string &name) 
{
	for( size_t i(0); i<linear_joint_vec.size(); i++ )
	{
		if( linear_joint_vec[i]->name == name )
			return linear_joint_vec[i];
	}
	return NULL;
}

// ----------------------------------------------------------------
//	Name：			BvhEditer::get_joint(const const char* name)
//	Description:	重载函数
//	Return Value:	Joint*
// ----------------------------------------------------------------
BvhEditer::part* BvhEditer::get_joint(const char* name ) 
{
	return BvhEditer::get_joint( std::string(name) );
}

//写BVH文件
void BvhEditer::recurs_write(ofstream &outFile)
{
	part joint;

	// HIERARCHY
	outFile << "HIERARCHY" << endl;
	outFile << "ROOT ";
	write_joint(outFile, root, 0);

	//write the motion data:
	//....
	outFile << "MOTION" << endl;
	outFile << "Frames:" << "\t" << frames_num << endl;
	outFile << "Frame Time:" << "\t" << frame_time << endl;

	outFile << fixed;
	for (size_t i(0); i<motion_data.size(); i++)
	{
		outFile << setprecision(6) << motion_data[i];
		if( (i+1)%channel_count == 0 )
			outFile << "\n";
		else
			outFile << "\t";
	}
}

//核心函数
void BvhEditer::write_joint(std::ofstream &outFile, part* node, int level)
{
	int i, j;
	outFile << node->name << endl;
	for(int i=0;i<level;i++)
		outFile << "\t";
	outFile << "{" << endl;
	level++;
	for(i=0;i<level;i++)
		outFile << "\t";

	outFile << fixed;
	outFile << "OFFSET "<< setprecision(6) << node->offset[0] << " ";
	outFile <<  setprecision(6) << node->offset[1] << " " << setprecision(6) << node->offset[2] << endl; 

	if(node->name != "Site")
	{
		for(i=0;i<level;i++)
			outFile << "\t";
		outFile << "CHANNELS " << node->channels.size();
		for(j=0;j<node->channels.size();j++)
		{
			outFile << " ";
			if ( node->channels[j]->type == part::Xpos) {		
				outFile << "Xposition";
			} else if (node->channels[j]->type == part::Ypos) {					
				outFile << "Yposition";
			} else if (node->channels[j]->type == part::Zpos) {					
				outFile << "Zposition";
			} else if (node->channels[j]->type == part::Zrot) {					
				outFile << "Zrotation";
			} else if (node->channels[j]->type == part::Yrot) {					
				outFile << "Yrotation";
			} else if (node->channels[j]->type == part::Xrot) {					
				outFile << "Xrotation";
			}
		}
		outFile << endl;

		for(j=0;j<node->children.size();j++)
		{
			for(i=0;i<level;i++)
				outFile << "\t";
			if(node->children[j]->name != "Site")
				outFile << "JOINT ";
			else
				outFile << "End ";
			write_joint(outFile, node->children[j], level);
		}
	}

	level--;
	for(i=0;i<level;i++)
		outFile << "\t";
	outFile << "}" << endl;
}