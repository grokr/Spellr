#ifndef __FILTER_H__
#define __FILTER_H__

class Filter 
{
public: 
	Filter(typo_long_num similarity=D_ZERO, typo_long_num warp=D_ZERO, typo_long_num frequency=D_ZERO, typo_long_num ngram=D_ZERO): 
	  _min_sim(similarity), _warp(warp), _freq(frequency), _ngram(ngram){}
	virtual ~Filter(){}

	typo_long_num sim() const { return _min_sim;}
	void sim(typo_long_num d){ _min_sim = d;}
	typo_long_num warp()  const { return _warp;}
	void warp(typo_long_num d){ _warp = d;}
	typo_long_num freq() const { return _freq;}
	void freq(typo_long_num d){ _freq=d;}
	typo_long_num ngram() const { return _ngram;}
	void ngram(typo_long_num d){ _ngram=d;}

private:
	typo_long_num _min_sim;
	typo_long_num _warp;
	typo_long_num _freq;
	typo_long_num _ngram; //, ignoreCase, levenshteinDist, soundexDist, debug};
};

	/*
	class Inferior
	{
	typo_long_num _sim;
	public:
	Inferior(typo_long_num s): _sim(s){}
	bool operator() (const Cand &c) { return c.similarity < _sim; }
	};
	*/


#endif //__FILTER_H__
