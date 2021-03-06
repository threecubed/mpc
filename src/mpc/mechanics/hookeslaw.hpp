/**
 *    \file hookeslaw.hpp
 *    \brief functions and corresponding functions objects for getting the
 * stress or strain tensor from the linear form of Hooke's Law
 *
 *    \author Anthony Torlucci
 *    \date 2018/10/14
 */

#ifndef MPC_HOOKESLAW_H
#define MPC_HOOKESLAW_H

// c++
#include <iostream>

// mpc
#include "mpc/core/cstypes.hpp"
#include "mpc/core/symmetrygrouptypes.hpp"
#include "mpc/core/stressstrain.hpp"
#include "mpc/core/stiffnesscompliance.hpp"

namespace mpc {
    namespace mechanics {

        // polymorphic template function object to calculate stress, strain, stiffness, or compliance
        // based on the linear form of Hooke's Law for infintesimal strain
        /**
         * \class HookesLawFunctionObject
         * \brief function object based on linear form of Hooke's Law
         *
         * Two specializations are provided ...
         */
        template <typename T, typename CS>
        struct HookesLawFunctionObject { /* specialized for stiffness and compliance types*/
            static_assert(std::is_floating_point<T>::value, "Type T must be of type float, double, or long double");
            //--static_assert(std::is_base_of<mpc::core::SymmetryGroupBase,S>::value, "S must be derived from mpc::core::SymmetryGroupBase.");
            static_assert(std::is_base_of<mpc::core::CSBase,CS>::value, "CS must be derived from mpc::core::CSBase.");
        };

        template <typename T>
        struct HookesLawFunctionObject<T,mpc::core::StiffnessType> {
            static_assert(std::is_floating_point<T>::value, "Type T must be of type float, double, or long double");
            //--static_assert(std::is_base_of<mpc::core::SymmetryGroupBase,S>::value, "S must be derived from mpc::core::SymmetryGroupBase.");
            //static_assert(std::is_base_of<mpc::core::CSBase,CS>::value, "CS must be derived from mpc::core::CSBase.");

            mpc::core::StressTensor<T> operator()(
                    mpc::core::StiffnessTensor<T>& stiffness,
                    mpc::core::StrainTensor<T>& strain) {
                // TODO: make const ref?
                mpc::core::StressTensor<T> stress = mpc::core::StressTensor<T>();
                // set components of stress
                T tmp;
                for (int i=0; i<3; ++i) {
                    for (int j=0; j<3; ++j) {
                        tmp = 0.0;
                        for (int k=0; k<3; ++k) {
                            for (int l=0; l<3; ++l) {
                                tmp += (stiffness.tensor(i,j,k,l) * strain.tensor(k,l));
                            }
                        }
                        stress.tensor(i,j) = tmp;
                    }
                }
                return stress;
            }
        };

        template <typename T>
        struct HookesLawFunctionObject<T,mpc::core::ComplianceType> {
            static_assert(std::is_floating_point<T>::value, "Type T must be of type float, double, or long double");
            //--static_assert(std::is_base_of<mpc::core::SymmetryGroupBase,S>::value, "S must be derived from mpc::core::SymmetryGroupBase.");
            //static_assert(std::is_base_of<mpc::core::CSBase,CS>::value, "CS must be derived from mpc::core::CSBase.");

            mpc::core::StrainTensor<T> operator ()(
                    mpc::core::ComplianceTensor<T>& compliance,
                    mpc::core::StressTensor<T>& stress) {
                // TODO: make const ref?
                mpc::core::StrainTensor<T> strain = mpc::core::StrainTensor<T>();
                // set components of strain
                T tmp;
                for (int i=0; i<3; ++i) {
                    for (int j=0; j<3; ++j) {
                        tmp = 0.0;
                        for (int k=0; k<3; ++k) {
                            for (int l=0; l<3; ++l) {
                                tmp += (compliance.tensor(i,j,k,l) * stress.tensor(k,l));
                            }
                        }
                        strain.tensor(i,j) = tmp;
                    }
                }
                return strain;
            }
        };

        // === polymorphic functions
        /**
         * \fn HookesLaw
         * \brief convenience function for calculating stress tensor from the stiffness and strain tensors
         */
        template <typename T>
        mpc::core::StressTensor<T> HookesLaw(mpc::core::StiffnessTensor<T>& c_ijkl, mpc::core::StrainTensor<T>& e_kl) {
            static_assert(std::is_floating_point<T>::value, "Type T must be of type float, double, or long double");
            //--static_assert(std::is_base_of<mpc::core::SymmetryGroupBase,S>::value, "S must be derived from mpc::core::SymmetryGroupBase.");
            mpc::mechanics::HookesLawFunctionObject<T,mpc::core::StiffnessType> hookes_law_fo;
            return hookes_law_fo(c_ijkl, e_kl);
        }

        /**
         * \fn HookesLaw
         * \brief convenience function for calculating strain tensor from the compliance and stress tensors
         */
        template <typename T>
        mpc::core::StrainTensor<T> HookesLaw(mpc::core::ComplianceTensor<T>& s_ijkl, mpc::core::StressTensor<T>& t_kl) {
            static_assert(std::is_floating_point<T>::value, "Type T must be of type float, double, or long double");
            //--static_assert(std::is_base_of<mpc::core::SymmetryGroupBase,S>::value, "S must be derived from mpc::core::SymmetryGroupBase.");
            mpc::mechanics::HookesLawFunctionObject<T,mpc::core::ComplianceType> hookes_law_fo;
            return hookes_law_fo(s_ijkl, t_kl);
        }

    }  // END namespace mechanics
}  // END namespace mpc

#endif // MPC_HOOKESLAW_H
