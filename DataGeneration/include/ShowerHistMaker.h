#ifndef HADSHOWERTUNING_ShowerHistMaker_h
#define HADSHOWERTUNING_ShowerHistMaker_h


class TDirectory;

namespace hadshowertuning {
    
    class Data;

    class ShowerHistMaker
    {
    public:
	void makeHist(TDirectory * dir,Data * data);
    };


}

#endif
