#ifndef _I_EXPORTER_FACTORY_H_
#define _I_EXPORTER_FACTORY_H_

#include "exporter/exporter.h"

#include <memory>

class IExporterFactory
{
public:
	virtual ~IExporterFactory() = default;
	virtual std::unique_ptr<IExporter> Create(const FileType& type) = 0;
};

#endif