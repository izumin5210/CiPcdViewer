//
// Created by Masayuki IZUMI on 7/19/16.
//

#ifndef CIPOINTCLOUDVIEWERAPP_CLOUDS_H
#define CIPOINTCLOUDVIEWERAPP_CLOUDS_H

#include <map>
#include <mutex>
#include <set>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include "glm/glm.hpp"

#include "Store.h"

#include "filter/PassThroughFilter.hpp"
#include "filter/VoxelFilter.hpp"
#include "filter/StatisticalOutlierRemovalFilter.hpp"

class Clouds : public Store {
public:
  using PointT        = pcl::PointXYZRGBA;
  using PointCloud    = pcl::PointCloud<PointT>;
  using PointCloudPtr = PointCloud::Ptr;
  using Key = std::string;

  struct UpdateCloudAction {
    Key key;
    PointCloudPtr cloud;
  };

  struct ChangeCloudVisibilityAction {
    Key key;
    bool visible;
  };

  struct RemoveCloudAction {
    Key key;
  };

  struct ClearCloudsAction {
  };

  struct UpdatePassThroughFilterParamsAction {
    std::string field;
    filter::PassThroughFilter<PointT>::Params params;
  };

  struct UpdateVoxelFilterParamsAction {
    filter::VoxelFilter<PointT>::Params params;
  };

  struct UpdateStatisticalOutlierRemovalFilterParamsAction {
    filter::StatisticalOutlierRemovalFilter<PointT>::Params params;
  };

  struct UpdateCloudLoadingProgressAction {
    Key key;
    unsigned long count;
    unsigned long max;
  };

  Clouds();

  std::map<Key, PointCloudPtr> clouds() const {
    return clouds_;
  };

  std::set<Key> hidden_clouds() const {
    return hidden_clouds_;
  }

  PointCloudPtr cloud() const {
    return cloud_;
  }

  size_t cloud_size() const {
    return cloud_size_;
  }

  size_t filtered_cloud_size() const {
    return filtered_cloud_size_;
  }

  filter::PassThroughFilter<PointT>::Params x_pass_through_filter_params() const {
    return x_pass_through_filter_.params();
  }

  filter::PassThroughFilter<PointT>::Params y_pass_through_filter_params() const {
    return y_pass_through_filter_.params();
  }

  filter::PassThroughFilter<PointT>::Params z_pass_through_filter_params() const {
    return z_pass_through_filter_.params();
  }

  filter::VoxelFilter<PointT>::Params voxel_filter_params() const {
    return voxel_filter_.params();
  }

  filter::StatisticalOutlierRemovalFilter<PointT>::Params sor_filter_params() const {
    return sor_filter_.params();
  }

  std::map<Key, glm::vec2> loading_progresses() const {
    return loading_progresses_;
  }


private:
  std::map<Key, PointCloudPtr> clouds_;
  std::set<Key> hidden_clouds_;

  PointCloudPtr cloud_;
  size_t cloud_size_;
  size_t filtered_cloud_size_;

  filter::PassThroughFilter<PointT> x_pass_through_filter_;
  filter::PassThroughFilter<PointT> y_pass_through_filter_;
  filter::PassThroughFilter<PointT> z_pass_through_filter_;
  filter::VoxelFilter<PointT> voxel_filter_;
  filter::StatisticalOutlierRemovalFilter<PointT> sor_filter_;

  std::map<Key, glm::vec2> loading_progresses_;

  std::mutex cloud_mutex_;

  void initializeConnections();
  void updatePointCloud();

  void onCloudUpdate(const UpdateCloudAction &action);
  void onCloudVisibilityChange(const ChangeCloudVisibilityAction &action);
  void onCloudRemove(const RemoveCloudAction &action);
  void onCloudsClear(const ClearCloudsAction &action);
  void onPassThroughFilterParamsUpdate(const UpdatePassThroughFilterParamsAction &action);
  void onVoxelFilterParamsUpdate(const UpdateVoxelFilterParamsAction &action);
  void onStatisticalOutlierRemovalFilterParamsUpdate(const UpdateStatisticalOutlierRemovalFilterParamsAction &action);
  void onCloudLoadingProgressUpdate(const UpdateCloudLoadingProgressAction &action);
};

#endif //CIPOINTCLOUDVIEWERAPP_CLOUDS_H