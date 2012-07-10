//An argument object passed while creating threads
#ifndef THREADARGUMENT_H_
#define THREADARGUMENT_H_
class threadArgument{
 private:
  int threadId;
 public:
  threadArgument(int id){
    threadId=id;
  }

  int getMyId(){
    return this->threadId;
  }
};

#endif /* THREADARGUMENT_H_ */
