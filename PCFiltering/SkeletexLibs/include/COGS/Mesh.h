/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <memory>

#include <COGS/PointCloud.h>
#include <COGS/Triangulation.h>
#include <COGS/MaterialModel.h>
#include <COGS/API.h>

namespace cogs
{
  /*!
    \brief A Mesh is structure containing a point-cloud a triangulation and a material. Not all of these are required.
    \note Each of the point-cloud, faces or material may be undefined.
  */
  struct COGS_API Mesh
  {
    std::shared_ptr<PointCloud> points = nullptr;
    std::shared_ptr<Triangulation> faces = nullptr;
    std::shared_ptr<MaterialModel> material = nullptr;

    [[nodiscard]]
    bool HasPoints() const;

    [[nodiscard]]
    bool HasFaces() const;

    [[nodiscard]]
    bool HasMaterial() const;

    //! Import from a file. Return success.
    virtual bool Import(const std::string &filename);

    //! Import to a file. Create a directory hierarchy if necessary. Return success.
    virtual bool Export(const std::string &filename) const;
  };

}
