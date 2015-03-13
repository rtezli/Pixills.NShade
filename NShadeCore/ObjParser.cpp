#include "stdafx.h"
#include "ObjParser.h"

HRESULT ObjParser::Parse(vector<NVertex>* vertices, vector<UINT>* indices, CHAR* fileName)
{
	auto isRightHand = true;

	auto fileLines = File::ReadFileLines(fileName);
	if (fileLines.empty())
	{
		return 0;
	}

	vector<string> parts;

	auto polygons = new vector<NPolygon>();

	for (unsigned long i = 0; i < fileLines.size(); i++)
	{
		auto vertex = new NVertex();
		string line = fileLines.at(i);
		boost::split(parts, line, boost::is_any_of(" "));

		if (parts.empty()) // if line is blank
		{
			continue;
		}

		if (parts[0] == "v")
		{
			XMFLOAT3 position;
			auto x = boost::lexical_cast<FLOAT>(parts.at(1));
			auto y = boost::lexical_cast<FLOAT>(parts.at(2));
			auto z = boost::lexical_cast<FLOAT>(parts.at(3));

			if (isRightHand)
			{
				z = z * -1.0f;
				position = XMFLOAT3(x, y, z);
			}
			else
			{
				position = XMFLOAT3(x, y, z);
			}

			vertex->Position = position;
			vertex->Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertices->push_back(*vertex);
		}
	}

	for (unsigned long i = 0; i < fileLines.size(); i++)
	{
		// UV coords
		auto vertex = new NVertex();
		if (parts[0] == "vt")
		{
			auto x = boost::lexical_cast<FLOAT>(parts.at(1));
			auto y = boost::lexical_cast<FLOAT>(parts.at(2));

			if (isRightHand)
			{
				y = 1.0f - y;
			}

			XMFLOAT2 uv(x, y);
			vertex->UV = uv;
		}
	}
	for (unsigned long i = 0; i < fileLines.size(); i++)
	{
		// Normals
		auto vertex = new NVertex();
		if (parts[0] == "vn")
		{
			auto x = boost::lexical_cast<FLOAT>(parts.at(1));
			auto y = boost::lexical_cast<FLOAT>(parts.at(2));
			auto z = boost::lexical_cast<FLOAT>(parts.at(3));

			if (isRightHand)
			{
				z = z * -1.0f;
			}

			XMFLOAT3 normal(x, y, z);
			vertex->Normal = normal;
		}
	}
	for (unsigned long i = 0; i < fileLines.size(); i++)
	{
		// Polygons
		if (parts[0] == "f")
		{
			auto i1 = boost::lexical_cast<UINT>(parts.at(1));
			auto i2 = boost::lexical_cast<UINT>(parts.at(2));
			auto i3 = boost::lexical_cast<UINT>(parts.at(3));
			auto i4 = 0;

			if (parts.size() == 4)
			{
				boost::lexical_cast<UINT>(parts.at(4));
			}

			NPolygon polygon;

			if (isRightHand)
			{
				polygon = { i3, i2, i1 };

				if (parts.size() == 4)
				{
					indices->push_back(i4);
				}

				indices->push_back(i3);
				indices->push_back(i2);
				indices->push_back(i1);

			}
			else
			{
				polygon = { i1, i2, i3 };

				indices->push_back(i1);
				indices->push_back(i2);
				indices->push_back(i3);

				if (parts.size() == 4)
				{
					indices->push_back(i4);
				}
			}

			polygons->push_back(polygon);
		}
	}
	return 0;
}