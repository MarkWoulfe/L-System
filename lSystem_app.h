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
    struct rule myRule;
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
      //myRule.head = 0;
      myRule.body = "";
      iterations = 0;
      angle = 0.f;
      read_file();
		}
    
    void read_file()
    {
      std::ifstream myFile;
      std::string line;
      myFile.open ("/Users/Woulfe/Desktop/tree1.txt");
      if (myFile.is_open())
      {
        printf("file opened\n");
        while (std::getline(myFile, line)){
          printf("%s \n",line.c_str());
        }
        
      }
      else {
        printf("failed to open\n");
      }
      myFile.close();
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