#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Tup.h"
#include <opencv2/opencv.hpp>
#include <map>

using std::vector;
using std::string;
using std::map;


enum illum_model {
    CONSTANT_COLOR,
    DIFFUSE,
    DIFFUSE_AND_SPECULAR
};

struct material {
    material(){};
    string name;
    tup<float, 3> ambient, diffuse, specular, emissive_coeficient;
    float specular_exponent, optical_density, transparency;
    illum_model illumination_model;
    string ambient_tex_file, diffuse_tex_file, specular_highlight_file;
    cv::Mat ambient_texture, diffuse_texture, specular_highlight_texture;
};

class vec3;
class polygon;

struct face {
    face()=default;
    face(
        tup<int, 3> vertex_indicies,
        tup<int, 3> vertex_tex_coord_indices,
        tup<int, 3> normal_indicies
    ) :
        vertex_indicies(vertex_indicies),
        vertex_tex_coord_indices(vertex_tex_coord_indices),
        normal_indicies(normal_indicies)
    {}
    tup<int, 3> vertex_indicies, vertex_tex_coord_indices, normal_indicies;
    friend std::ostream& operator<<(std::ostream& os, const face& self) {
        os << "face< verts=" << self.vertex_indicies << ", uv=" << self.vertex_tex_coord_indices << ", norms=" << self.normal_indicies << " >";
        return os;
    }
};
 
class meshgroup {
public:
    meshgroup() {} 
    meshgroup(vector<vec3>* vertexes, vector<vec3>* uv_vertexes, vector<vec3>* vertex_normals):
    vertexes(vertexes), 
    uv_vertexes(uv_vertexes), 
    vertex_normals(vertex_normals)
    {}
    meshgroup(const meshgroup& rhs): 
    vertexes(rhs.vertexes), 
    faces(rhs.faces), 
    uv_vertexes(rhs.uv_vertexes), 
    vertex_normals(rhs.vertex_normals),
    material_data(rhs.material_data)
    {}

    void get_gl_verts(vector<vec3> vertexes, vector<float>* mut_verts);
    void get_gl_vert_inds(vector<vec3> vertexes, vector<unsigned int>* mut_inds);
    vector<polygon> get_polygons(vector<vec3> vertexes);

    vector<vec3>* vertexes;
    vector<vec3>* uv_vertexes;
    vector<vec3>* vertex_normals;
    material* material_data;
    vector<face> faces;
};

class mesh {
public:
    mesh(){}
    mesh(map<std::string, meshgroup>* groups, map<std::string, material*> materials, vector<vec3>* vertexes, vector<vec3>* uv_vertexes, vector<vec3>* vertex_normals):
    groups(groups),
    materials(materials),
    vertexes(vertexes),
    uv_vertexes(uv_vertexes),
    vertex_normals(vertex_normals)
    {}
    mesh(const mesh& rhs):
    groups(rhs.groups),
    materials(rhs.materials),
    vertexes(rhs.vertexes),
    uv_vertexes(rhs.uv_vertexes),
    vertex_normals(rhs.vertex_normals)
    {}
    ~mesh(){
        delete vertexes;
        delete uv_vertexes;
        delete vertex_normals;
        delete groups;
        for (auto& [k,v] : materials) {
            delete v;
        }
    }
    static mesh*  from_obj(string file_path);
    map<std::string, meshgroup>* groups;
    map<std::string, material*> materials;
    // VVV THESE SHOULD BE HEAP ALLOCATED
    vector<vec3>* vertexes;
    vector<vec3>* uv_vertexes;
    vector<vec3>* vertex_normals;
private:
    static vector<tup<int, 3>> parse_face(vector<string> tokens);

    // RETURNS A HEAP ALLOCATED POINTER
    static map<std::string, material*> get_materials(string file_path);
};

inline string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
} 
  
inline std::vector<std::string> split(std::string const &input) { 
    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), 
                                 std::istream_iterator<std::string>());
    return ret;
}

template<typename T>
void print_vec(vector<T> in);

inline size_t sub_str_ind(const std::string& haystack, const std::string& needle, int nth)
{
    size_t  pos = 0;
    int     cnt = 0;

    while( cnt != nth )
    {
        pos+=1;
        pos = haystack.find(needle, pos);
        if ( pos == std::string::npos )
            return -1;
        cnt++;
    }
    return pos;
}