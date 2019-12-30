//pybind11
#define strdup _strdup //if windows.
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <utility>
#include <algorithm>
#include <cstddef>
#include "../shellCPP.hpp"

class shellCombined{
    private:
    shell::shellCalc calc;
    shell::shell s;

    public:
    shellCombined(const double v0, const double caliber, const double krupp, const double mass,
    const double normalization, const double cD, const std::string& name, const double threshold, const double fuseTime){
        s.setValues(v0, caliber, krupp, mass, normalization, cD, name, threshold, fuseTime);
    }

    void calcImpact(){
        calc.calculateImpact(s, false);
    }

    void calcPostPen(double thickness, std::vector<double> angles){
        calc.calculatePostPen(thickness, s, angles);
    }

    void printImpact(){
        if(s.completedImpact){
            s.printImpactData();
        }else{
            throw std::runtime_error("Impact data not generated");
        }
    }

    void printPostPen(){
        if(s.completedPostPen){
            s.printPostPenData();
        }else{
            throw std::runtime_error("PostPen data not generated");
        }
    }

    pybind11::array_t<double> getImpact(){
        if(s.completedImpact){
            constexpr std::size_t sT = sizeof(double);
            auto result = pybind11::array(pybind11::buffer_info(
                s.getImpactPtr(0, 0),                       /* Pointer to data (nullptr -> ask NumPy to allocate!) */
                sT,                                         /* Size of one item */
                pybind11::format_descriptor<double>::value, /* Buffer format */
                2,                                          /* How many dimensions? */
                std::vector<std::size_t>{ shell::impact::maxColumns, s.impactSizeAligned }, /* Number of elements for each dimension */
                std::vector<std::size_t>{ s.impactSizeAligned * sT, sT}                     /* Strides for each dimension */
            ));
            return result;
        }else{
            throw std::runtime_error("Impact data not generated");
        }
    }

    pybind11::array_t<double> getPostPen(){
        if(s.completedPostPen){
            constexpr std::size_t sT = sizeof(double);
            auto result = pybind11::array(pybind11::buffer_info(
                s.getPostPenPtr(0, 0),                      /* Pointer to data (nullptr -> ask NumPy to allocate!) */
                sT,                                         /* Size of one item */
                pybind11::format_descriptor<double>::value, /* Buffer format */
                2,                                          /* How many dimensions? */
                std::vector<std::size_t>{ shell::post::maxColumns, s.postPenSize },  /* Number of elements for each dimension */
                std::vector<std::size_t>{ s.postPenSize * sT, sT}                    /* Strides for each dimension */
            ));
            return result;
        }else{
            throw std::runtime_error("PostPen data not generated");
        }
    }

};

//Testline: s = shell(780, .460, 2574, 1460, 6, .292, "Yamato", 76.0, .033 )
PYBIND11_MODULE(pythonwrapper, m){
    pybind11::class_<shellCombined>(m, "shell", pybind11::buffer_protocol())
        .def(pybind11::init<double, double, double, double, double, double, std::string& , double, double>())
        .def("calcImpact", &shellCombined::calcImpact)
        .def("calcPostPen", &shellCombined::calcPostPen)
        .def("getImpact", &shellCombined::getImpact, pybind11::return_value_policy::reference)
        .def("getPostPen", &shellCombined::getPostPen, pybind11::return_value_policy::reference)
        .def("printImpact", &shellCombined::printImpact)
        .def("printPostPen", &shellCombined::printPostPen);
    /*
    pybind11::enum_<shellCombined.s::stdDataIndex>(m, "stdDataIndex", py::arithmetic())
        .value("distance", shellCombined::shell::stdDataIndex::distance)
        .value("launchA" , shellCombined::shell::stdDataIndex::launchA)
        .value("impactAHR", shellCombined::shell::stdDataIndex::impactAHR)
        .value("impactAHD", shellCombined::shell::stdDataIndex::impactAHD)
        .value("impactV", shellCombined::shell::stdDataIndex::impactV)
        .value("rawPen", shellCombined::shell::stdDataIndex::rawPen)
        .value("ePenH", shellCombined::shell::stdDataIndex::ePenH)
        .value("ePenHN", shellCombined::shell::stdDataIndex::ePenHN)
        .value("impactADD", shellCombined::shell::stdDataIndex::impactADD)
        .value("ePenD", shellCombined::shell::stdDataIndex::ePenD)
        .value("ePenDN", shellCombined::shell::stdDataIndex::ePenDN)
        .value("tToTarget", shellCombined::shell::stdDataIndex::tToTarget)
        .value("tToTargetA", shellCombined::shell::stdDataIndex::tToTargetA)
        .export_values();*/
};

