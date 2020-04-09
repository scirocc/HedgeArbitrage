#include"getData.h"

namespace getData {
    void getData1(int beginT,std::string intervel,std::string contract) {
        Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化
        //需要特别注意，python脚本中如果print中文，会引起导入模块错误
        if (!Py_IsInitialized()) {
            printf("初始化失败！");
            std::terminate();
        }
        auto dir=getEXEFileDir();
        dir=dir.substr(0,dir.rfind("\\cmake-build-"));
        std::string str1="sys.path.append('";
        std::string str2="/src/')";
        std::string envpath=str1+dir+str2;

        PyRun_SimpleString("import sys");
        PyRun_SimpleString(envpath.c_str());
        //这一步很重要，标识出要引入的python文件路径
        PyObject *pModule = nullptr;//声明变量
        pModule = PyImport_ImportModule("getData");//这里是要调用的文件名
        if (pModule == nullptr)//注意，除了未找到文件外，若模块没有正常运行结束，也会返回空指针
        {
            std::cout << "something wrong happend" << std::endl;
            std::terminate();
        }
        PyObject *pFunc = nullptr;
        pFunc = PyObject_GetAttrString(pModule, "main");//这里是要调用的函数名
        PyObject* args = PyTuple_New(3);
        PyObject* arg1 = PyLong_FromLong(beginT);
        PyObject* arg2 = PyUnicode_FromString(intervel.c_str());
        PyObject* arg3 = PyUnicode_FromString(contract.c_str());

        PyTuple_SetItem(args, 0, arg1);
        PyTuple_SetItem(args, 1, arg2);
        PyTuple_SetItem(args, 2, arg3);
        PyEval_CallObject(pFunc,args);//调用带参数无返回值的python函数
//
//    PyObject* pRet = PyObject_CallObject(pFunc3, args2);//调用函数
//    int res = 0;
//    PyArg_Parse(pRet, "i", &res);//转换返回类型
        if (PyErr_Occurred() != nullptr){
            PyObject *ptype, *pvalue, *ptraceback;
            PyErr_Fetch(&ptype, &pvalue, &ptraceback);
            auto pStrErrorMessage = PyUnicode_AsUTF8(pvalue);
            std::cout<<"py file error happen!"<<std::endl;;
            std::cout<<pStrErrorMessage<<std::endl;
            std::terminate();
        }
        Py_Finalize();
        // 与初始化对应
    }

    using mytype=std::map<long long,std::tuple<double,double,double,double,double,double>>;
    std::vector<mytype> getData2() {
        auto vFile=MyGlob("./data/*.js");
        std::vector<mytype> vResult;
        for (auto& file :vFile){
            std::string contractName=file.substr(file.rfind('/')+1,file.rfind('.')-file.rfind('/')-1);
            auto js=jsonLoad_slow(file);
            auto hTimeTuple=js.get<std::map<std::string,std::tuple<double,double,double,double,double,double>>>();
            mytype hTimeTuple_;
            for(const auto& iter:hTimeTuple){
                hTimeTuple_[stoll(iter.first)]=iter.second;
            }
            vResult.emplace_back(hTimeTuple_);
        }
        return vResult;
    }


    void getExternalData(const std::string& contract,const int& beginT){//外盘期货
        auto myOracleDealer=MyOracleDealer("192.168.0.18:1521/orcl","market","1");


        myOracleDealer.execute( "select code,mdate,mtime,closeprice from sh_day where mdate>20200404");
        if (myOracleDealer.next()){
            auto t=myOracleDealer.fetchone<int,std::string>();
            std::cout<< std::get<0>(t)<<std::endl;
            std::cout<< std::get<1>(t)<<std::endl;
        }
        auto x=myOracleDealer.fetchall<int,std::string>();
        for (auto y:x){
            std::cout<< std::get<1>(y)<<std::endl;

        }


    }


}