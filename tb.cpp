#include "verilated.h"
#include "../testbench.h"
#include <verilated_vcd_c.h>
#include <fstream>
#include <queue>
#include <sstream>

#include "Vedge_det.h"

#define XSTRING(str) STRING(str)
#define STRING(str) #str

int main(int argc, char** argv, char** env) {

  Verilated::commandArgs(argc, argv);
  TESTBENCH<Vedge_det> *tb = new TESTBENCH<Vedge_det>();
  std::string str_buff;
  std::queue<int> din_test_v, edge_p_v;

#ifdef USER_FILE
  std::string str =  XSTRING(USER_FILE);
#else
  std::string str = getenv("VLSI");
  str+="/edge_det/test2.txt";
#endif

std::ifstream ifp(str, std::ios::in);

  //------------------------------------------------------------------
  // load test vectors
  //------------------------------------------------------------------
  // DIN test vector
  std::string line;
  std::getline(ifp, line);
  std::istringstream is(line);
  while (is>>str_buff)
  {
    din_test_v.push(std::stoi(str_buff,nullptr,0));
  }
  //------------------------------------------------------------------

int min_size = din_test_v.size() ;
#ifdef USER_FILE
  int size = (min_size > 32) ? 32 : min_size;
#ifdef OUTPUT
  std::string waves_path = XSTRING(OUTPUT);
#else
  std::string waves_path = "./edge_det";
#endif
  waves_path+="/waves.vcd";
  tb->opentrace(waves_path.c_str());
#else
  int size = din_test_v.size();
#endif

#ifndef USER_FILE
  std::string waves_path = "./edge_det";
  waves_path+="/waves.vcd";
  tb->opentrace(waves_path.c_str());
#endif

tb->reset();

#ifdef USER_FILE
  
  // posedge_trigger from user_file and pop from stack
  tb->m_core->posedge_trigger = din_test_v.front();
  din_test_v.pop();
  
  // negedge_trigger from user_file and pop from stack
  tb->m_core->negedge_trigger = din_test_v.front();
  din_test_v.pop();
  
  // din from user_file and pop from stack

  for (int i=0; i < size-2; i++)
  {
    tb->m_core->din = din_test_v.front();
    din_test_v.pop();

    if(tb->m_core->posedge_trigger & 0x1)
    {
      edge_p_v.push(din_test_v.front());
    }
    if(tb->m_core->negedge_trigger & 0x1)
    {
      edge_p_v.push(din_test_v.front());
    }
    tb->tick();
  }
#endif

#ifndef USER_FILE
   
  int posedge_trigger, negedge_trigger;
  unsigned int dout;

  tb->tick();

  for(posedge_trigger = 0; posedge_trigger < 2; posedge_trigger++){
    
    for(negedge_trigger = 0; negedge_trigger < 2; negedge_trigger++){
      
      tb->m_core->posedge_trigger = posedge_trigger;

      tb->m_core->negedge_trigger = negedge_trigger;
      
      for(int i=1; i<5;i++){
          
          tb->m_core->din = i & 1;
          din_test_v.push(tb->m_core->din);
          tb->tick();

          if(posedge_trigger == 0 && negedge_trigger == 0){
            dout = 0;
            if(tb->m_core->edge_p != dout)
              tb->test_failed("Dout when posedge and negedge trigger aren't detected is not like expected", dout, tb->m_core->edge_p);
          }

          else if (posedge_trigger == 0 && negedge_trigger == 1){
            dout = (tb->m_core->din)^1;
            if(tb->m_core->edge_p !=dout)
              tb->test_failed("Dout when only negedge_trigger is detected is not like expected", dout, tb->m_core->edge_p);
          }

          else if (posedge_trigger == 1 && negedge_trigger == 0){
            dout = (tb->m_core->din) & 1;
            if(tb->m_core->edge_p !=dout)
              tb->test_failed("Dout when only posedge_trigger is detected is not like expected", dout, tb->m_core->edge_p);
          }

          else if(posedge_trigger == 1 && negedge_trigger == 1) {
            dout = 1;
            if(tb->m_core->edge_p !=dout)
              tb->test_failed("Dout when both posedge and negedge trigger are detected is not like expected", dout, tb->m_core->edge_p);
          }

          else{
            if(tb->m_core->edge_p !=dout)
            tb->test_failed("FAIL");
          }
       
      }
      tb->tick();
    }
    tb->tick();
  } 

#endif
tb->tick();
#ifdef USER_FILE
  tb->close();
  std::cout<<"WAVEFORM GENERATED!"<<std::endl;
#else
  tb->close();
  std::cout<<"TEST PASSED!"<<std::endl;
#endif
  return 0;
}