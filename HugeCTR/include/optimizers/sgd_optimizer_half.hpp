/*
 * Copyright (c) 2020, NVIDIA CORPORATION.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "HugeCTR/include/optimizer.hpp"

namespace HugeCTR {

/**
 * Adam optimizer
 */
class SgdOptimizerHalf : public Optimizer {
 public:
  /**
   * Constructor of SgdOptimizerHalf.
   * names of hyper-parameters are the same as in Algorithm 1 of Adam paper (arXiv:1412.6980)
   * @param weight weights to be updated
   * @param wgrad gradient for weights
   * @param device_id the id of GPU where update kernel is launched
   * @param lr learning rate
   # @param scaler scaler factor for mixed precision
   */
  SgdOptimizerHalf(const std::shared_ptr<GeneralBuffer<float>>& weight,
		   const std::shared_ptr<GeneralBuffer<__half>>& wgrad, 
		   int device_id, float lr = 0.001f, 
		   float scaler = 1.f)
    : Optimizer(weight,device_id, lr, scaler), wgrad_(wgrad) {
      if (weight_->get_num_elements() != wgrad_->get_num_elements()) {
        CK_THROW_(Error_t::WrongInput, "weight_.get_num_elements() != wgrad_.get_num_elements(): " + std::to_string(weight_->get_num_elements()) + " vs " + std::to_string(wgrad_->get_num_elements()));
      }
  }

  /**
   * update the weights using gradient
   * @param stream cuda stream used by update kernel
   */
  void update(cudaStream_t stream) override;

 private:
  std::shared_ptr<GeneralBuffer<__half>> wgrad_;
};

}  // namespace HugeCTR
