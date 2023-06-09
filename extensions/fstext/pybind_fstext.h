
#ifndef KALPY_PYBIND_FSTEXT_H_
#define KALPY_PYBIND_FSTEXT_H_

#include "pybind/kaldi_pybind.h"

#include "fst/script/fst-class.h"
#include "fst/script/info-impl.h"
#include "fst/script/print-impl.h"
#include "fst/vector-fst.h"
#include "fst/fst.h"


template <typename W>
void pybind_arc_impl(py::module& m, const std::string& class_name,
                     const std::string& class_help_doc = "") {
  using PyClass = fst::ArcTpl<W>;
  using Weight = typename PyClass::Weight;
  using Label = typename PyClass::Label;
  using StateId = typename PyClass::StateId;

  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def(py::init<Label, Label, Weight, StateId>(), py::arg("ilabel"),
           py::arg("olabel"), py::arg("weight"), py::arg("nextstate"))
      .def(py::init<const PyClass&>(), py::arg("weight"))
      .def_readwrite("ilabel", &PyClass::ilabel)
      .def_readwrite("olabel", &PyClass::olabel)
      .def_readwrite("weight", &PyClass::weight)
      .def_readwrite("nextstate", &PyClass::nextstate)
      .def("__str__",
           [](const PyClass& arc) {
             std::ostringstream os;
             os << "(ilabel: " << arc.ilabel << ", "
                << "olabel: " << arc.olabel << ", "
                << "weight: " << arc.weight << ", "
                << "nextstate: " << arc.nextstate << ")";
             return os.str();
           })
      .def_static("Type", &PyClass::Type, py::return_value_policy::reference);
}

template <typename A>
void pybind_state_iterator_base_impl(py::module& m,
                                     const std::string& class_name,
                                     const std::string& class_help_doc = "") {
  using PyClass = fst::StateIteratorBase<A>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def("Done", &PyClass::Done, "End of iterator?")
      .def("Value", &PyClass::Value, "Returns current state (when !Done()).")
      .def("Next", &PyClass::Next, "Advances to next state (when !Done()).")
      .def("Reset", &PyClass::Reset, "Resets to initial condition.");
}

template <typename A>
void pybind_state_iterator_data_impl(py::module& m,
                                     const std::string& class_name,
                                     const std::string& class_help_doc = "") {
  using PyClass = fst::StateIteratorData<A>;
  py::class_<PyClass, std::unique_ptr<PyClass, py::nodelete>>(
      m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def_readwrite("base", &PyClass::base,
                     "Specialized iterator if non-zero.")
      .def_readwrite("nstates", &PyClass::nstates,
                     "Otherwise, the total number of states.");
}

template <typename A>
void pybind_arc_iterator_base_impl(py::module& m, const std::string& class_name,
                                   const std::string& class_help_doc = "") {
  using PyClass = fst::ArcIteratorBase<A>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def("Done", &PyClass::Done, "End of iterator?")
      .def("Value", &PyClass::Value, "Returns current arc (when !Done()).",
           py::return_value_policy::reference)
      .def("Next", &PyClass::Next, "Advances to next arc (when !Done()).")
      .def("Position", &PyClass::Position, "Returns current position.")
      .def("Reset", &PyClass::Reset, "Resets to initial condition.")
      .def("Seek", &PyClass::Seek, "Advances to arbitrary arc by position.")
      .def("Flags", &PyClass::Flags, "Returns current behavorial flags.")
      .def("SetFlags", &PyClass::SetFlags, "Sets behavorial flags.");
}

template <typename A>
void pybind_arc_iterator_data_impl(py::module& m, const std::string& class_name,
                                   const std::string& class_help_doc = "") {
  using PyClass = fst::ArcIteratorData<A>;
  py::class_<PyClass, std::unique_ptr<PyClass, py::nodelete>>(
      m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def_readwrite("base", &PyClass::base,
                     "Specialized iterator if non-zero.")
      .def_readwrite("arcs", &PyClass::arcs, "Otherwise arcs pointer")
      .def_readwrite("narcs", &PyClass::narcs, "arc count")
      .def_readwrite("ref_count", &PyClass::ref_count,
                     "reference count if non-zero.");
}

template <typename FST>
void pybind_state_iterator_impl(py::module& m, const std::string& class_name,
                                const std::string& class_help_doc = "") {
  using PyClass = fst::StateIterator<FST>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<const FST&>(), py::arg("fst"))
      .def("Done", &PyClass::Done)
      .def("Value", &PyClass::Value)
      .def("Next", &PyClass::Next)
      .def("Reset", &PyClass::Reset);
}

template <typename FST>
void pybind_arc_iterator_impl(py::module& m, const std::string& class_name,
                              const std::string& class_help_doc = "") {
  using PyClass = fst::ArcIterator<FST>;
  using StateId = typename FST::StateId;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<const FST&, StateId>(), py::arg("fst"), py::arg("s"))
      .def("Done", &PyClass::Done)
      .def("Value", &PyClass::Value, py::return_value_policy::reference)
      .def("Next", &PyClass::Next)
      .def("Reset", &PyClass::Reset)
      .def("Seek", &PyClass::Seek, py::arg("a"))
      .def("Position", &PyClass::Position)
      .def("Flags", &PyClass::Flags)
      .def("SetFlags", &PyClass::SetFlags);
}

template <typename FST>
void pybind_mutable_arc_iterator_impl(py::module& m,
                                      const std::string& class_name,
                                      const std::string& class_help_doc = "") {
  using PyClass = fst::MutableArcIterator<FST>;
  using StateId = typename PyClass::StateId;

  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<FST*, StateId>(), py::arg("fst"), py::arg("s"))
      .def("Done", &PyClass::Done)
      .def("Value", &PyClass::Value, py::return_value_policy::reference)
      .def("SetValue", &PyClass::SetValue, py::arg("arc"))
      .def("Next", &PyClass::Next)
      .def("Reset", &PyClass::Reset)
      .def("Seek", &PyClass::Seek, py::arg("a"))
      .def("Position", &PyClass::Position)
      .def("Flags", &PyClass::Flags)
      .def("SetFlags", &PyClass::SetFlags);
}

template <typename A>
void pybind_vector_fst_impl(py::module& m, const std::string& class_name,
                            const std::string& class_help_doc = "") {
  using PyClass = fst::VectorFst<A>;
  using Arc = typename PyClass::Arc;
  using StateId = typename PyClass::StateId;
  using State = typename PyClass::State;

  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def(py::init<const fst::Fst<Arc>&>(), py::arg("fst"))
      .def(py::init<const PyClass&, bool>(), py::arg("fst"),
           py::arg("safe") = false)
      .def("Start", &PyClass::Start)
      .def("Final", &PyClass::Final, py::arg("s"))
      .def("SetStart", &PyClass::SetStart, py::arg("s"))
      .def("SetFinal", &PyClass::SetFinal, py::arg("s"), py::arg("weight"))
      .def("SetProperties", &PyClass::SetProperties, py::arg("props"),
           py::arg("mask"))
      .def("AddState", (StateId (PyClass::*)()) & PyClass::AddState)
      .def("AddArc", (void (PyClass::*)(StateId, const Arc&))(&PyClass::AddArc), py::arg("s"), py::arg("arc"))
      .def("DeleteStates", (void (PyClass::*)(const std::vector<StateId>&)) &
                               PyClass::DeleteStates,
           py::arg("dstates"))
      .def("DeleteStates", (void (PyClass::*)()) & PyClass::DeleteStates,
           "Delete all states")
      .def("DeleteArcs",
           (void (PyClass::*)(StateId, size_t)) & PyClass::DeleteArcs,
           py::arg("state"), py::arg("n"))
      .def("DeleteArcs", (void (PyClass::*)(StateId)) & PyClass::DeleteArcs,
           py::arg("s"))
      .def("ReserveStates", &PyClass::ReserveStates, py::arg("s"))
      .def("ReserveArcs", &PyClass::ReserveArcs, py::arg("s"), py::arg("n"))
      .def("InputSymbols", &PyClass::InputSymbols,
           "Returns input label symbol table; return nullptr if not "
           "specified.",
           py::return_value_policy::reference)
      .def("OutputSymbols", &PyClass::OutputSymbols,
           "Returns output label symbol table; return nullptr if not "
           "specified.",
           py::return_value_policy::reference)
      .def("MutableInputSymbols", &PyClass::MutableInputSymbols,
           "Returns input label symbol table; return nullptr if not "
           "specified.",
           py::return_value_policy::reference)
      .def("MutableOutputSymbols", &PyClass::MutableOutputSymbols,
           "Returns output label symbol table; return nullptr if not "
           "specified.",
           py::return_value_policy::reference)
      .def("SetInputSymbols", &PyClass::SetInputSymbols, py::arg("isyms"))
      .def("SetOutputSymbols", &PyClass::SetOutputSymbols, py::arg("osyms"))
      .def("NumStates", &PyClass::NumStates)
      .def("NumArcs", &PyClass::NumArcs, py::arg("s"))
      .def("NumInputEpsilons", &PyClass::NumInputEpsilons, py::arg("s"))
      .def("NumOutputEpsilons", &PyClass::NumOutputEpsilons, py::arg("s"))
      .def("Properties", &PyClass::Properties, py::arg("mask"), py::arg("test"))
      .def("Type", &PyClass::Type, "FST typename",
           py::return_value_policy::reference)
      .def("Copy", &PyClass::Copy,
           "Get a copy of this VectorFst. See Fst<>::Copy() for further "
           "doc.",
           py::arg("safe") = false, py::return_value_policy::take_ownership)
      .def_static("Read",
                  // clang-format off
            overload_cast_<std::istream&, const fst::FstReadOptions&>()(&PyClass::Read),
                  // clang-format on
                  "Reads a VectorFst from an input stream, returning nullptr "
                  "on error.",
                  py::arg("strm"), py::arg("opts"),
                  py::return_value_policy::take_ownership)
      .def_static("Read", overload_cast_<const std::string&>()(&PyClass::Read),
                  "Read a VectorFst from a file, returning nullptr on error; "
                  "empty "
                  "filename reads from standard input.",
                  py::arg("filename"), py::return_value_policy::take_ownership)
      .def("Write",
           // clang-format off
            (bool (PyClass::*)(std::ostream&, const fst::FstWriteOptions&)const)&PyClass::Write,
           // clang-format on
           "Writes an FST to an output stream; returns false on error.",
           py::arg("strm"), py::arg("opts"))
      .def("Write",
           (bool (PyClass::*)(const std::string&) const) & PyClass::Write,
           "Writes an FST to a file; returns false on error; an empty\n"
           "filename results in writing to standard output.",
           py::arg("filename"))
      .def_static("WriteFst", &PyClass::template WriteFst<PyClass>,
                  py::arg("fst"), py::arg("strm"), py::arg("opts"))
      .def("InitStateIterator", &PyClass::InitStateIterator,
           "For generic state iterator construction (not normally called "
           "directly by users). Does not copy the FST.",
           py::arg("data"))
      .def("InitArcIterator", &PyClass::InitArcIterator,
           "For generic arc iterator construction (not normally called "
           "directly by users). Does not copy the FST.",
           py::arg("s"), py::arg("data"));
}

void pybind_fstext_context_fst(py::module &);
void pybind_fstext_deterministic_fst(py::module &);
void pybind_fstext_deterministic_lattice(py::module &);
void pybind_fstext_determinize_star(py::module &);
void pybind_fstext_epsilon_property(py::module &);
void pybind_fstext_factor(py::module &);
void pybind_fstext_fstext_utils(py::module &);
void pybind_fstext_grammar_context_fst(py::module &);
void pybind_fstext_lattice_utils(py::module &);
void pybind_fstext_pre_determinize(py::module &);
void pybind_fstext_prune_special(py::module &);
void pybind_fstext_rand_fst(py::module &);
void pybind_fstext_remove_eps_local(py::module &);
void pybind_kaldi_fst_io(py::module &);
void pybind_fst_types(py::module &);
void init_fstext(py::module &);
#endif  // KALPY_PYBIND_FSTEXT_H_
