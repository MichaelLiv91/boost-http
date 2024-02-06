#pragma once

#include "BasicTypes.h"
#include <boost/thread/thread.hpp>

#include "gdal_priv.h"
#include "cpl_conv.h"
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

namespace Services
{
	
	struct tiffData {
		int pixel_width;
		int pixel_height;
		double bbox_width;
		double bbox_height;
		double bbox_0;
		double bbox_1;
	};

	class GeoCalculationService
	{

	public:
		~GeoCalculationService() {
			delete[] m_elevationMatrix;
			delete s_Instance;
		}

		void Init();
		float GetElevation(BasicTypes::Position _pos);
		static GeoCalculationService* getInstance();

	private:
		
		GeoCalculationService() : m_elevationLoader{}, m_tiffData{}, m_elevationMatrix{ nullptr }, m_condition{}, m_lock{}, m_isReady{ false }{};

		void loadData();
		void getBoundingBox(GDALDataset* _tiffFile, double bbox[4]);
		void getRasters(GDALDataset* _tiffFile);
		boost::thread* m_elevationLoader;
		tiffData m_tiffData;
		float* m_elevationMatrix;
		std::condition_variable m_condition;
		std::mutex m_lock;
		bool m_isReady;
		static std::once_flag s_flag;
		static GeoCalculationService* s_Instance;
	};

}