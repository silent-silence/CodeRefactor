//枚举类型
enum {
  Enum1,
  Enum2,
  Enum3
} TestEnum;

void TestFun(char *str){
  *str="1234567890";
}

int main()
{
  //结构体
  struct {
    int a;
    float b;
    double c;
    char d;
  } TestStruct, *StructPointer;
  StructPointer = &TestStruct;
  StructPointer->a = 1;
  TestStruct.a = 1;
  
  //类型重命名
  typedef int TestInt;
  TestInt testNum = 0;
  
  //for语句
  for(int i =0; i<10; ++i){
    if(i==5)
      continue;
    i++;
  }
  
  //while语句
  while(!testNum){
    if(testNum == 10){
      break;
    }
    else{
      testNum++;
    }
  }

  //Switch语句
  switch(testNum){
  case 0:

  case 1:
    break;
  case 2:
    break;
  default:
    break;
  }
  
  //数组
  char array[1024]="0987654321";
  TestFun(array);
  
  return 0;
}
