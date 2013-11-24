//libraries for reading to/from files
#include <fstream>

namespace octet
{
	class lSystem_app : public app
	{
    
    struct rule {
      char head;
      std::string body;
    };
    
    std::string axiom;
    dynarray<rule> myRules;
    int iterations;
    float angle;
    
	public:
    
		lSystem_app(int argc, char **argv) : app(argc, argv)
		{
      
		}
    
		void app_init()
		{
      printf("This is the example's init function!\n");
      
      axiom = "";
      iterations = 0;
      angle = 0.f;
      
      read_file("../assets/lsystem/tree1.txt");
      read_file("../assets/lsystem/tree2.txt");
      read_file("../assets/lsystem/tree3.txt");
      read_file("../assets/lsystem/tree4.txt");
      read_file("../assets/lsystem/tree5.txt");
      read_file("../assets/lsystem/tree6.txt");
		}
    
    void read_file(std::string file)
    {
      std::ifstream myFile;
      std::string line;
      myFile.open (file);
      if (myFile.is_open())
      {
        printf("file opened\n");
        while (std::getline(myFile, line)){
          
          if (line.compare("axiom") == 0){
            std::getline(myFile, line);
            axiom = line;
          }
          
          if (line.compare("rule") == 0){
            std::getline(myFile, line);
            rule r;
            r.head = line[0];
            for (int i = 3; i < line.length(); i++){
              r.body.push_back(line[i]);
            }
            myRules.push_back(r);
          }
          
          if (line.compare("iterations") == 0){
            std::getline(myFile, line);
            iterations = atoi(line.c_str());
          }
          
          if (line.compare("angle") == 0){
            std::getline(myFile, line);
            angle = atof(line.c_str());
          }
          
        }
        
        printf("Axiom: %s \n", axiom.c_str());
        
        for (int i=0; i<myRules.size();i++){
        printf("Rule: %c", myRules[i].head);
        printf("->%s \n", myRules[i].body.c_str());
        }
        
        printf("Iterations: %i \n", iterations);
        printf("Angle: %f \n", angle);
        
      }
      else {
        printf("failed to open\n");
      }
      myFile.close();
      
      myRules.resize(0);
      
      printf("file closed\n");
    }
    
		void draw_world(int x, int y, int w, int h)
		{
      // set a viewport - includes whole window area
      glViewport(x, y, w, h);
      
      // clear the background to black
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
			//printf("This is the examples's drawing function!\n");
		}
    
	};
}