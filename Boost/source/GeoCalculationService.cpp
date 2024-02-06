#pragma once
#include "GeoCalculationService.h"
#include <fstream> 
#include <filesystem>

#include <math.h>
#include <nlohmann/json.hpp>
#include <functional>

using json = nlohmann::json;
constexpr float M_TO_FEET = 3.281F;



std::once_flag Services::GeoCalculationService::s_flag{};
Services::GeoCalculationService* Services::GeoCalculationService::s_Instance = nullptr;

Services::GeoCalculationService* Services::GeoCalculationService::getInstance()
{
	if (s_Instance == nullptr)
	{
		
		std::call_once(s_flag, []() {
			s_Instance = new GeoCalculationService{};
			s_Instance->Init();
			});
	}

	return s_Instance;
}

void Services::GeoCalculationService::Init()
{
	
	m_elevationLoader = new boost::thread([this]() {
		this->loadData();
		});
}


float Services::GeoCalculationService::GetElevation(BasicTypes::Position position)
{
	std::unique_lock<std::mutex> lock(m_lock);
	m_condition.wait(lock, [this] { return m_isReady; });
	
	auto widthPct = (position.longitude - m_tiffData.bbox_0) / m_tiffData.bbox_width;
	auto heightPct = (position.latitude - m_tiffData.bbox_1) / m_tiffData.bbox_height;
	if (m_elevationMatrix == nullptr || widthPct < 0 || heightPct < 0)
	{
		return 0;
	}
	int xPx = static_cast<int>(std::floor(m_tiffData.pixel_width * widthPct));
	int yPx = static_cast<int>(std::floor(m_tiffData.pixel_height * (1 - heightPct)));
	auto res = m_elevationMatrix[yPx * m_tiffData.pixel_width + xPx] * M_TO_FEET;
	return res;
}

void Services::GeoCalculationService::loadData()
{
	
	GDALAllRegister();

	// Your GDAL code here
	// Open the TIFF file for reading

	GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen("layers/cy_comp_dem.tif", GA_ReadOnly));
	if (dataset == nullptr) {
		printf("Error opening TIFF file\n");
	}

	double bbox[4] = {};
	m_tiffData.pixel_width = dataset->GetRasterXSize();
	m_tiffData.pixel_height = dataset->GetRasterYSize();
	getBoundingBox(dataset, bbox);
	m_tiffData.bbox_0 = bbox[0];
	m_tiffData.bbox_1 = bbox[1];

	m_tiffData.bbox_width = bbox[2] - bbox[0];
	m_tiffData.bbox_height = bbox[3] - bbox[1];

	getRasters(dataset);
	// Read pixel values or perform other operations here

	// Close the dataset
	GDALClose(dataset);



	fprintf(stderr, "Load data finished\n");
	GDALDestroyDriverManager();

	// Notify waiting threads that the work is done
	m_isReady = true;
	std::unique_lock<std::mutex> lock(m_lock);
	lock.unlock();
	m_condition.notify_one();
	
}

void Services::GeoCalculationService::getBoundingBox(GDALDataset* _tiffFile, double bbox[4])
{
	double adfGeoTransform[6];
	if (_tiffFile->GetGeoTransform(adfGeoTransform) == CE_None) {
		// Extract bounding box coordinates
		bbox[0] = adfGeoTransform[0];
		bbox[1] = adfGeoTransform[3] + adfGeoTransform[5] * _tiffFile->GetRasterYSize();
		bbox[2] = adfGeoTransform[0] + adfGeoTransform[1] * _tiffFile->GetRasterXSize();
		bbox[3] = adfGeoTransform[3];
	}
}

void Services::GeoCalculationService::getRasters(GDALDataset* _tiffFile)
{
	int bandIndex = 1;
	GDALRasterBand* band = _tiffFile->GetRasterBand(bandIndex);
	if (band == nullptr) {
		printf("Error getting raster band\n");
		GDALClose(_tiffFile);
	}

	// Get band dimensions
	int width = band->GetXSize();
	int height = band->GetYSize();

	// Read all samples into an array of the correct type
	m_elevationMatrix = new float[width * height];  
	float* temp = new float[width * height];  

	// Read data into the array
	band->RasterIO(GF_Read, 0, 0, width, height, temp, width, height, GDT_Float32, 0, 0);

	for (auto j = 0; j < height; j++) {
		for (auto i = 0; i < width; i++) {
			m_elevationMatrix[i + j * width] = temp[i + j * width];
			if (m_elevationMatrix[i + j * width] < 0) {
				m_elevationMatrix[i + j * width] = 0;
			}
		}
	}

	if (temp != nullptr)
	{
		delete[] temp;
	}

}
