#include "ProgramOptions.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const SimulationType& type)
{
  switch(type)
  {
    case SPLATS:
      os << "splats";
      break;
    case SMOKE:
      os << "smoke";
      break;
    case CLOUDS:
      os << "clouds";
      break;
  }

  return os;
}

std::istream& operator>>(std::istream& is, SimulationType& type)
{
  std::string token;
  is >> token;
  if(token == "splats") { type = SPLATS; return is; }
  if(token == "smoke")  { type = SMOKE; return is; }
  if(token == "clouds")  { type = CLOUDS; return is; }

  throw std::invalid_argument("bad simulation");
  return is;
}

ProgramOptions parseOptions(int argc, char* argv[])
{
  namespace po = boost::program_options;

  ProgramOptions options;

  po::options_description poWindow("Window options");
  poWindow.add_options()
    ("windowWidth", po::value<unsigned>(&options.windowWidth)->default_value(800), "window width")
    ("windowHeight", po::value<unsigned>(&options.windowHeight)->default_value(800), "window height")
    ("exportImages", po::value<bool>(&options.exportImages)->default_value(false), "export simulation to a set of PNG files")
  ;

  po::options_description poSim("Simulation options");
  poSim.add_options()
    ("simType,s", po::value<SimulationType>(&options.simType)->default_value(SPLATS), "type of simulation (splats, smoke)")
    ("deltaTime,t", po::value<float>(&options.dt)->default_value(0.1f), "time step for the simulation")
    ("simWidth", po::value<unsigned>(&options.simWidth)->default_value(1024), "simulation width (must be a power of 2)")
    ("simHeight", po::value<unsigned>(&options.simHeight)->default_value(1024), "simulation height (must be a power of 2)")
    ("jacobi-iterations", po::value<unsigned>(&options.jacobiIterations)->default_value(50), "number of iterations for the Jacobi method")
    ("mc-revert", po::value<float>(&options.mcRevert)->default_value(0.05), "revert parameter for the maccormack advection scheme")
  ;

  po::options_description po_options("sim [options]");
  po_options.add(poWindow).add(poSim).add_options()
    ("help,h", "display this message")
  ;

  try
  {
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(po_options).run(), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
      std::cout << po_options;
      std::exit(0);
    }
  }
  catch (std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    std::cout << po_options;
    std::exit(1);
  }

  return options;
}
