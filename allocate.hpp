/**
 * allocate.hpp - allocate base class, extend me to build new
 * allocate classes.  This object has several useful features
 * and data structures, namely the set of all source kernels and
 * all the kernels within the graph.  There is also a list of 
 * all the currently allocated FIFO objects within the streaming
 * graph.  This is primarily for instrumentation puposes.
 * @author: Jonathan Beard
 * @version: Tue Sep 16 20:20:06 2014
 * 
 * Copyright 2014 Jonathan Beard
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _ALLOCATE_HPP_
#define _ALLOCATE_HPP_  1


#include "kernel.hpp"
#include "port_info.hpp"
#include "fifo.hpp"
#include <set>

class Map;

class Allocate
{
public:
   /**
    * Allocate - base constructor, really doesn't do too much
    * save for setting the global variables all_kernels and 
    * source_kernels from the Map object.  
    * @param map - Map&
    */
   Allocate( Map &map );
   
   /**
    * destructor 
    */
   virtual ~Allocate();

   /**
    * run - implement this function to create a new allocator, will
    * be run inside a thread so exits when done but if run-time monitoring
    * is desired then this is the place to do it.
    */
   virtual void run() = 0;

   /** 
    * notReady - returns true if not yet ready.  The run function
    * should call the protected function setReady() when the initial
    * allocation is complete so that the scheduling threads can 
    * commence scheduling.
    * @return bool - true when not ready
    */
   bool notReady();
protected:
   /**
    * initialize - internal method to be used within the run method
    * takes care of the initialization using the already allocated
    * FIFO object passed as a param.  This function will throw 
    * an exception if either port (src or dst) have already been
    * allocated.
    * @param   src - PortInfo&
    * @param   dst - PortInfo&
    * @param   fifo - FIFO*
    * @throws  PortDoubleInitializeException - if either port is already initialized.
    */
   void initialize( PortInfo &src, PortInfo &dst, FIFO *fifo );
  
   /**
    * setReady - call within the implemented run function to signal
    * that the initial allocations have been completed.
    */
   void setReady();

   /** both convenience structs, hold exactly what the names say **/
   std::set< Kernel* > &source_kernels;
   std::set< Kernel* > &all_kernels;
   
   /** 
    * keeps a list of all currently allocated FIFO objects,
    * set from within the initialize function.
    */
   std::set< FIFO*   > allocated_fifo;

private:
   bool ready = false;
};
#endif /* END _ALLOCATE_HPP_ */
