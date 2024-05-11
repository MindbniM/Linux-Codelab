#include"processpool.hpp"
void Hint()
{
    std::cout<<"Hint:"<<std::endl;
    std::cout<<"\t ./processpool [processnumbers]"<<std::endl;
}

int main(int args,char *argv[])
{
    if(args!=2)
    {
        Hint();
    }
    int P_number=std::stoi(argv[1]);
    if(P_number<=0) return P_number_error;
    processpool ps(P_number);
    works<void()> wr;
    wr.Addwork({"work111",[]{std::cout<<"working  111"<<std::endl;}});
    wr.Addwork({"work222",[]{std::cout<<"working  222"<<std::endl;}});
    wr.Addwork({"work333",[]{std::cout<<"working  333"<<std::endl;}});
    wr.Addwork({"work444",[]{std::cout<<"working  444"<<std::endl;}});
    wr.Addwork({"work555",[]{std::cout<<"working  555"<<std::endl;}});

    ps.CreateProcess(wr);
    int cun=10;
    while(cun)
    {
        ps.SendTaskCode(wr);
        cun--;
    }
    return 0;
}