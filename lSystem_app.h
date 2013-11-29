
#include <fstream>

namespace octet
{
  
  class line {
    
    mat4t modelToWorld;
    
    vec4 colour;
    
  public:
    
    
    void init(mat4t mtw, vec4 lineColour) {
      
      modelToWorld = mtw;
      colour = lineColour;
      
    }
    
    void render(color_shader &shader, mat4t &cameraToWorld) {
      
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);
      
      shader.render(modelToProjection, colour);
      
      static const float vertices[] = {
        0, 0, 0,
        0, 2, 0,
      };
      
      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)vertices);
      glEnableVertexAttribArray(attribute_pos);
      
      glDrawArrays(GL_LINES, 0, 2);
    }
  };
  
	class lSystem_app : public app
	{
    
    bool pressed;
    
    mat4t modelToWorld;
    
    mat4t cameraToWorld;
    
    color_shader color_shader_;
    
    struct rule {
      char head;
      std::string body;
    };
    
    std::string axiom;
    dynarray<rule> myRules;
    int iterations, maxIterations;
    float angle;
    std::string currentOutput;
    std::string currentInput;
    
    dynarray<mat4t>modelToWorlds;
    dynarray<line>lines;
    
    vec4 leafColour;
    
	public:
    
		lSystem_app(int argc, char **argv) : app(argc, argv)
		{
      
		}
    
    void app_init()
		{
      pressed = false;
      
      color_shader_.init();
      
      cameraToWorld.loadIdentity();
      
      axiom = "";
      iterations = maxIterations = 0;
      angle = 0.f;
      leafColour = vec4(1,0,1,1);
      
      read_file("../assets/lsystem/tree1.txt");
      interpret_rule();
      build_tree();
      
      cameraToWorld.translate(0, 250, 300);
		}
    
    void read_file(std::string file)
    {
      std::ifstream myFile;
      std::string line;
      myFile.open (file);
      if (myFile.is_open())
      {
        //rules for the push, pops, and angle of rotation position
        rule r;
        r.head = '[';
        r.body = '[';
        myRules.push_back(r);
        
        r.head = ']';
        r.body = ']';
        myRules.push_back(r);
        
        r.head = '+';
        r.body = '+';
        myRules.push_back(r);
        
        r.head = '-';
        r.body = '-';
        myRules.push_back(r);
        
        r.body = "";
        
        printf("file opened\n");
        while (std::getline(myFile, line)){
          
          if (line.compare("axiom") == 0){
            std::getline(myFile, line);
            axiom = line;
          }
          
          if (line.compare("rule") == 0){
            std::getline(myFile, line);
            r.head = line[0];
            for (int i = 3; i < line.length(); i++){
              r.body.push_back(line[i]);
            }
            myRules.push_back(r);
            r.body = "";
          }
          
          if (line.compare("iterations") == 0){
            std::getline(myFile, line);
            iterations = atoi(line.c_str());
            maxIterations = iterations;
            
          }
          
          if (line.compare("angle") == 0){
            std::getline(myFile, line);
            angle = atof(line.c_str());
          }
          
        }
        
      }
      else {
        printf("failed to open\n");
      }
      myFile.close();
      
      printf("file closed\n\n");
    }
    
    void interpret_rule()
    {
      
      currentInput = axiom;
      
      //for all iterations
      for(int i=0; i < iterations; i++){
        //go through current iteration(string)
        for(int j = 0; j < currentInput.length(); j++){
          //go through all the rules
          for (int k = 0; k < myRules.size(); k++){
            //compare in the current iteration any charachters to a rule
            if (currentInput[j] == myRules[k].head){
              //adds onto existing string
              currentOutput.insert(currentOutput.length(),myRules[k].body);
              
            }
            
          }
          
        }
        //change input to new iteration
        currentInput = currentOutput;
        //reset string for next iteration
        currentOutput = "";
        
      }
      
      
    }
    
    void forward(vec4 colour){
      
      line myLine;
      
      myLine.init(modelToWorld, colour);
      
      lines.push_back(myLine);
      
      modelToWorld.translate(0,2,0);
      
    }
    
    void rotateClockwise(float angle){
      
      modelToWorld.rotateZ(angle);
      
    }
    
    void rotateAntiClockwise(float angle){
      
      
      modelToWorld.rotateZ(-angle);
      
    }
    
    void push(){
      
      modelToWorlds.push_back(modelToWorld);
      
    }
    
    void pop(){
      
      modelToWorld = modelToWorlds[modelToWorlds.size() - 1];
			modelToWorlds.pop_back();
      
    }
    
    void build_tree(){
      
      modelToWorld.loadIdentity();
      
      for (int i=0; i<currentInput.length(); i++){
        
        if (currentInput[i] == 'F'){
          forward(vec4(0,1,0,1));
        }
        
        if (currentInput[i] == 'X'){
          forward(leafColour);
        }
        
        if (currentInput[i] == '+'){
          rotateClockwise(angle);
        }
        
        if (currentInput[i] == '-'){
          rotateAntiClockwise(angle);
        }
        
        if (currentInput[i] == '['){
          push();
        }
        
        if (currentInput[i] == ']'){
          pop();
        }
        
      }
      
    }
    
    void draw_tree(){
      for (int i = 0; i < lines.size(); i++){
        lines[i].render(color_shader_, cameraToWorld);
      }
    }
    
    void key_presses(){
      
      if (is_key_down('Q')){
				cameraToWorld.translate(0, -7, -10);
      }
      if (is_key_down('E')){
				cameraToWorld.translate(0, 7, 10);
      }
      
      if(is_key_down(key_left)){
        lines.resize(0);
        angle-=1;
        build_tree();
      }
      
      if(is_key_down(key_right)){
        lines.resize(0);
        angle+=1;
        build_tree();
      }
      
      if(is_key_down('[')){
        
      }
      
      else if(is_key_down(']')){
        
      }
      
      if(is_key_down('U')){
        if(!pressed){
          lines.resize(0);
          leafColour = vec4(1,1,1,1);
          build_tree();
        }
        pressed = true;
      }
      
      else if(is_key_down('I')){
        if(!pressed){
          lines.resize(0);
          leafColour = vec4(1,1,0,1);
          build_tree();
        }
        pressed = true;
      }
      
      else if(is_key_down('O')){
        if(!pressed){
          lines.resize(0);
          leafColour = vec4(1,0,1,1);
          build_tree();
        }
        pressed = true;
      }
      
      else if(is_key_down('P')){
        if(!pressed){
          lines.resize(0);
          leafColour = vec4(0,1,1,1);
          build_tree();
        }
        pressed = true;
      }
      
      else if(is_key_down(key_up)){
        if(!pressed){
          lines.resize(0);
          if(iterations < maxIterations)iterations++;
          interpret_rule();
          build_tree();
        }
        pressed = true;
      }
      
      else if(is_key_down(key_down)){
        if(!pressed){
          lines.resize(0);
          if(iterations > 1) iterations--;
          interpret_rule();
          build_tree();
        }
        pressed = true;
      }
      
      else if(is_key_down('1')){
        if(!pressed){
          cameraToWorld.loadIdentity();
          lines.resize(0);
          myRules.resize(0);
          axiom = "";
          iterations = maxIterations = 0;
          angle = 0.f;
          read_file("../assets/lsystem/tree1.txt");
          interpret_rule();
          build_tree();
          cameraToWorld.translate(0, 230, 300);
        }
        pressed = true;
      }
      
      else if(is_key_down('2')){
        if(!pressed){
          cameraToWorld.loadIdentity();
          lines.resize(0);
          myRules.resize(0);
          axiom = "";
          iterations = maxIterations = 0;
          angle = 0.f;
          read_file("../assets/lsystem/tree2.txt");
          interpret_rule();
          build_tree();
          cameraToWorld.translate(0, 65, 75);
        }
        pressed = true;
      }
      
      else if(is_key_down('3')){
        if(!pressed){
          cameraToWorld.loadIdentity();
          lines.resize(0);
          myRules.resize(0);
          axiom = "";
          iterations = maxIterations = 0;
          angle = 0.f;
          read_file("../assets/lsystem/tree3.txt");
          interpret_rule();
          build_tree();
          cameraToWorld.translate(0, 65, 75);
        }
        pressed = true;
      }
      
      else if(is_key_down('4')){
        if(!pressed){
          cameraToWorld.loadIdentity();
          lines.resize(0);
          myRules.resize(0);
          axiom = "";
          iterations = maxIterations = 0;
          angle = 0.f;
          read_file("../assets/lsystem/tree4.txt");
          interpret_rule();
          build_tree();
          cameraToWorld.translate(0, 230, 300);
        }
        pressed = true;
      }
      
      else if(is_key_down('5')){
        if(!pressed){
          cameraToWorld.loadIdentity();
          lines.resize(0);
          myRules.resize(0);
          axiom = "";
          iterations = maxIterations = 0;
          angle = 0.f;
          read_file("../assets/lsystem/tree5.txt");
          interpret_rule();
          build_tree();
          cameraToWorld.translate(0, 230, 300);
        }
        pressed = true;
      }
      
      else if(is_key_down('6')){
        if(!pressed){
          cameraToWorld.loadIdentity();
          lines.resize(0);
          myRules.resize(0);
          axiom = "";
          iterations = maxIterations = 0;
          angle = 0.f;
          read_file("../assets/lsystem/tree6.txt");
          interpret_rule();
          build_tree();
          cameraToWorld.translate(0, 80, 90);
        }
        pressed = true;
      }
      
      else if(is_key_down('7')){
        if(!pressed){
          cameraToWorld.loadIdentity();
          lines.resize(0);
          myRules.resize(0);
          axiom = "";
          iterations = maxIterations = 0;
          angle = 0.f;
          read_file("../assets/lsystem/kockislands.txt");
          interpret_rule();
          build_tree();
          cameraToWorld.translate(-40, 40, 100);
        }
        pressed = true;
      }
      
      else if(is_key_down('8')){
        if(!pressed){
          cameraToWorld.loadIdentity();
          lines.resize(0);
          myRules.resize(0);
          axiom = "";
          iterations = maxIterations = 0;
          angle = 0.f;
          read_file("../assets/lsystem/morekock.txt");
          interpret_rule();
          build_tree();
          cameraToWorld.translate(120, -80, 200);
        }
        pressed = true;
      }
      
      else if(is_key_down('9')){
        if(!pressed){
          cameraToWorld.loadIdentity();
          lines.resize(0);
          myRules.resize(0);
          axiom = "";
          iterations = maxIterations = 0;
          angle = 0.f;
          read_file("../assets/lsystem/evenmorekock.txt");
          interpret_rule();
          build_tree();
          cameraToWorld.translate(-5, -15, 30);
        }
        pressed = true;
      }
      
      else{
        pressed = false;
      }
      
    }
    
		void draw_world(int x, int y, int w, int h)
		{
      
      glViewport(x, y, w, h);
      
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      glEnable(GL_DEPTH_TEST);
      
      draw_tree();
      
      key_presses();
      
		}
    
	};
}