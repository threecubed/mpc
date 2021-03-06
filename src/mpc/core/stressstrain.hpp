/**
 *    \file stressstrain.hpp
 *    \brief stress and strain tensor classes; part of the core data structures
 *
 *    \author Anthony Torlucci
 *    \date 2018/10/14
 */

#ifndef MPC_STRESSSTRAIN_H
#define MPC_STRESSSTRAIN_H

// c++
#include <cassert>
#include <set>

// blitz
#include <blitz/array.h>

// mpc
#include "tensorcomponentindex.hpp"
#include "tensorcomponent.hpp"
#include "tensorcomponentindexaliases.hpp"

namespace mpc {
    namespace core {

        /**
         * \class TensorRank2Interface
         * \brief simple interface class for tensors of rank 2
         */
        template <typename T>
        struct TensorRank2Interface {
            static_assert(std::is_floating_point<T>::value, "Type T must be of type float, double, or long double");
            virtual void SetComponents(std::set< mpc::core::TensorRank2Component<T> >& components, bool ApplySymmetry) = 0;
            // NOTE: the ApplySymmetry parameter here is used when you know that the tensor should be symmetrical, e.g. the Stress and Strain tensors.  However, the stress and strain tensors can be broken into symmetrical and asymetrical parts; for this reason, this parameter is provided so that the same class can be used for both parts and this parameter will determine if the symmetrical components need to be generated.
        };

        /**
         * \class TensorRank2Base
         * \brief simple base class for tensors of rank 2
         */
        template <typename T>
        struct TensorRank2Base : TensorRank2Interface<T> {
            blitz::Array<T,2> tensor = blitz::Array<T,2>(3,3, blitz::ColumnMajorArray<2>());
            TensorRank2Base() {
                tensor = 0.0;
            }
        };

        /**
         * \class StressTensor
         * \brief stress tensor class with function to set the components
         */
        template <typename T>
        struct StressTensor : TensorRank2Base<T> {
            using TensorRank2Base<T>::tensor;

            /**
            * \fn void SetComponents(std::set< mpc::core::TensorRank2Component<T> >&, bool)
            * \brief set the components of the underlying tensor data member
            */
            void SetComponents(std::set< mpc::core::TensorRank2Component<T> >& components, bool ApplySymmetry=false)
            {
                assert(components.size() < 10);

                /*
                 * for each component in the vector, get another vector of the possible
                 *     aliases, e.g. 11 has no aliases where as 12 will have an alias 21
                 *     due to the symmetry of the tensor.  Each alias in the set is
                 *     of type blitz::TinyVector<int,2> which is used to index the
                 *     tensor and assign the value from the component.
                 */
                for (auto c : components) {
                    if(ApplySymmetry) {
                        auto index_aliases = mpc::core::TensorRank2IndexAliases(c.Index());
                        for (auto i : index_aliases) {
                            // for each possible alias, set the value
                            auto tv = mpc::core::TensorRank2ComponentIndex::ToBlitzTinyVector(i);
                            tensor(tv) = c.Value();
                        }

                    } else {
                        // for each possible alias, set the value
                        auto index = c.Index();
                        tensor(index.FirstIndex(), index.SecondIndex()) = c.Value();
                    }
                }
            }
        };

        /**
         * \class StrainTensor
         * \brief strain tensor class with function to set the components
         */
        template <typename T>
        struct StrainTensor : TensorRank2Base<T> {
            using TensorRank2Base<T>::tensor;

            /**
            * \fn void SetComponents(std::set< mpc::core::TensorRank2Component<T> >&, bool)
            * \brief set the components of the underlying tensor data member
            */
            void SetComponents(std::set< mpc::core::TensorRank2Component<T> >& components, bool ApplySymmetry=false)
            {
                assert(components.size() < 10);

                /*
                 * for each component in the vector, get another vector of the possible
                 *     aliases, e.g. 11 has no aliases where as 12 will have an alias 21
                 *     due to the symmetry of the tensor.  Each alias in teh vector is
                 *     of type blitz::TinyVector<int,2> which is used to index the
                 *     tensor and assign the value from the component.
                 */
                for (auto c : components) {
                    if(ApplySymmetry) {
                        auto index_aliases = mpc::core::TensorRank2IndexAliases(c.Index());
                        for (auto i : index_aliases) {
                            // for each possible alias, set the value
                            auto tv = mpc::core::TensorRank2ComponentIndex::ToBlitzTinyVector(i);
                            tensor(tv) = c.Value();
                        }

                    } else {
                        // for each possible alias, set the value
                        auto index = c.Index();
                        tensor(index.FirstIndex(), index.SecondIndex()) = c.Value();
                    }
                }
            }
        };

    }  // namespace core
}  // namespace mpc

#endif // MPC_STRESSSTRAIN_H
