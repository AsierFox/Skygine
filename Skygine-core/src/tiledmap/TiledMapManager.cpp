#include "TiledMapManager.h"

TiledMapManager* TiledMapManager::s_instance = nullptr;

TiledMapManager::TiledMapManager()
{
}

TiledMapManager* TiledMapManager::getInstance()
{
	if (nullptr == TiledMapManager::s_instance)
	{
		TiledMapManager::s_instance = new TiledMapManager;
	}
	return TiledMapManager::s_instance;
}

// TODO Refactor this method
void TiledMapManager::load(std::string mapId, std::string resourcesDirPath, std::string filename)
{
	// TODO Create method isAlreadyLoaded()
	if (this->m_maps.count(mapId) > 0)
	{
		spdlog::debug("[TiledMapManager::load] The map with id '{0}' is already created!", mapId);
		return;
	}

	std::ifstream mapFile(resourcesDirPath + filename);
	nlohmann::json mapJson = nlohmann::json::parse(mapFile);

	int mapTileSize = mapJson["tilewidth"];
	int mapTileCols = mapJson["width"];
	int mapTileRows = mapJson["height"];

	std::vector<Tileset> mapTilesets;

	for (unsigned int i = 0; i < mapJson["tilesets"].size(); i++)
	{
		Tileset tileset;
		nlohmann::json tilesetJson = mapJson["tilesets"][i];

		int tileCount = tilesetJson["tilecount"];
		tileset.m_firstId = tilesetJson["firstgid"];
		tileset.m_lastId  = tileset.m_firstId + (tileCount - 1);
		tileset.m_name    = tilesetJson["name"].get<std::string>();
		tileset.m_tileSize = mapTileSize;
		tileset.m_totalCols = tilesetJson["columns"];
		tileset.m_totalRows = tileCount / tileset.m_totalCols;
		tileset.m_resourcePath = resourcesDirPath + tilesetJson["image"].get<std::string>();

		mapTilesets.push_back(tileset);
	}

	std::vector<Layer*> mapLayers;

	for (unsigned int i = 0; i < mapJson["layers"].size(); i++)
	{
		nlohmann::json layerJson = mapJson["layers"][i];

		if (std::string("tilelayer").compare(layerJson["type"].get<std::string>()) == 0)
		{
			std::vector<std::vector<int> > tileMapIds;
			std::vector<int> rowIds;

			for (unsigned int j = 0; j < layerJson["data"].size(); j++)
			{
				rowIds.push_back(layerJson["data"][j]);

				if ((j + 1) % mapTileCols == 0)
				{
					tileMapIds.push_back(rowIds);
					rowIds.clear();
				}
			}

			mapLayers.push_back(new TileLayer(tileMapIds, mapTilesets));
			
			tileMapIds.clear();
		}
		else if (std::string("objectgroup").compare(layerJson["type"].get<std::string>()) == 0)
		{
		}
	}

	TiledMap* tiledMap = new TiledMap(mapTileSize, mapTileCols, mapTileRows, mapLayers);
	this->m_maps.insert({ mapId, tiledMap });

	spdlog::debug("[TiledMapManager::load] The map with id '{0}' loaded successfully!", mapId);
}

void TiledMapManager::dispose(std::string mapId)
{
	this->m_maps["mapid"]->dispose();
}

TiledMap* TiledMapManager::getMap(std::string mapId)
{
	return this->m_maps[mapId];
}