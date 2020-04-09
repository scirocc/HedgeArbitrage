#include <getData.h>
#include <myOracle.hpp>

void getExternalData(const std::string& contract,const int& beginT){//Õ‚≈Ã∆⁄ªı
//    auto myOracleDealer=MyOracleDealer("192.168.0.18:1521/orcl","market","1");
//    myOracleDealer.execute((std::string &) "select mdate, code from sh_day");
//    if (myOracleDealer.next()){
//        auto t=myOracleDealer.fetchone<int,ocilib::ostring>();
//        std::cout<< std::get<0>(t)<<std::endl;
//        std::cout<< std::get<1>(t)<<std::endl;
//    }


}


int main(){
    system("chcp 65001 > nul");

//    getData::getData1(20200315,"5s","sc");
//    auto vResult=getData::getData2();
//    auto myOracleDealer=MyOracleDealer("192.168.0.8:1521/orcl","market","1");
    auto myOracleDealer=MyOracleDealer("192.168.0.8:1521/orcl","market","1");
myOracleDealer.execute( "select mdate, code from sh_day");
if (myOracleDealer.next()){
    auto t=myOracleDealer.fetchone<int,std::string>();
//    std::cout<< std::get<0>(t)<<std::endl;
//    std::cout<< std::get<1>(t)<<std::endl;
}

    return 0;

}