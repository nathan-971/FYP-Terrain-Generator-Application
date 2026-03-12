#ifndef _EXPORTER_FACTORY_H_
#define _EXPORTER_FACTORY_H_

#include "exporter/iexporter.h"
#include "utils/iexporterfactory.h"

class ExporterFactory : public IExporterFactory
{
public:
	std::unique_ptr<IExporter> Create(const FileType& type) override;
};

#endif