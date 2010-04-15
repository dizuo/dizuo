#ifndef WAVEAPPLYER_H_
#define WAVEAPPLYER_H_

#include "WaveFunc.h"
#include "Model.h"

class WaveApplyer
{
	CModel*		pWaveModel;
public:
	WaveApplyer() 
		: pWaveModel(0)
	{};
	~WaveApplyer() 
	{
		if (pWaveModel)
		{
			delete pWaveModel;
			pWaveModel = 0;
		}
	};

	///////////////////////////////////////////////////////////////////////////////
	// wobble the vertex in and out along normal
	///////////////////////////////////////////////////////////////////////////////
	void operator()(CModel& dstModel, float time)
	{
		if (!pWaveModel)
		{
			pWaveModel = new CModel(dstModel);
		}
		WaveFunc wave;
		wave.func = FUNC_SIN;   // sine wave function
		wave.amp = 0.08f;       // amplitude
		wave.freq = 2.0f;       // cycles/sec
		wave.phase = 0;         // horizontal shift
		wave.offset = 0;        // vertical shift

		Real waveLength = 1.5f;
		Real height;

		for(int i=0; i < dstModel.GetVertNum(); ++i)
		{
			// get source from original vertex array
			CVertex v = pWaveModel->GetVertex(i);

			const Real *vertex = v.v;
			const Real *norm = v.n;

			// compute phase (horizontal shift)
			wave.phase = (vertex[0] + vertex[2] + vertex[3]) / waveLength;

			height = wave.update(time);

			dstModel.GetVertex(i).v[0] = vertex[0] + height*norm[0];
			dstModel.GetVertex(i).v[1] = vertex[1] + height*norm[1];
			dstModel.GetVertex(i).v[2] = vertex[2] + height*norm[2];
		}
	}

};
#endif
