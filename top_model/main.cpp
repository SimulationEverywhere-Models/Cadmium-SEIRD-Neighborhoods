//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Atomic model headers
#include "../atomics/accumulator.hpp"
#include "../atomics/infective.hpp"
#include "../atomics/exposed.hpp"
#include "../atomics/susceptible.hpp"

//C++ headers
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;
using namespace cadmium;


using TIME = float;

/***** Define input port for coupled models *****/
struct inp_1 : public in_port<float>{};
struct inp_2 : public in_port<float>{};
/***** Define output ports for coupled model *****/
struct outp_1 : public out_port<float>{};
struct outp_2 : public out_port<float>{};

/*************** Loggers *******************/
    static ofstream out_messages("simulation_results/SEIRD_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("simulation_results/SEIRD_output_state.txt");
    struct oss_sink_state{
        static ostream& sink(){          
            return out_state;
        }
    };
    
    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;
/******************************************************/

int main(int argc, char ** argv) {
   
    float mortality_V;
	float infectivity_period_V;
	float dt_V;
	float incubation_period_V;
	int total_population_V;
	int initial_infective_V;
	float transmission_rate_V;

    float mortality_R;
    float infectivity_period_R;
    float dt_R;
    float incubation_period_R;
    int total_population_R;
    int initial_infective_R;
    float transmission_rate_R;

	ifstream inputReader_V;
    ifstream inputReader_R;


    inputReader_V.open("input_data/input_vanier.txt");
	//reading input from file
	if(inputReader_V.is_open()){
		std::string line; //store read in line
		
		//these blocks read the input for each variable and ensure that they have been provided
		getline(inputReader_V, line);
		line = line.substr(line.find("=")+1);
		if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){ //the comparison to 13 here is to the ascii value of the carriage return character
			assert(false && "Please check the mortality input");
			//abort();
		}else{
			//cout<<"this is the line: "<<line<<endl;
			//cout<<"got here"<<endl;
			mortality_V = std::stof(line);
			//cout<<"got here"<<endl;
		}
		
		getline(inputReader_V, line);
		line = line.substr(line.find("=")+1);
		if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
			assert(false && "Please check the infectivity period input");
		}else{
			infectivity_period_V = std::stof(line);
		}
		
		getline(inputReader_V, line);
		line = line.substr(line.find("=")+1);
		if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
			assert(false && "Please check the dt input");
		}else{
			dt_V = std::stof(line);
		}
		
		getline(inputReader_V, line);
		line = line.substr(line.find("=")+1);
		if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
			assert(false && "Please check the incubation period input");
		}else{
			incubation_period_V = std::stof(line);
		}
		
		getline(inputReader_V, line);
		line = line.substr(line.find("=")+1);
		if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
			assert(false && "Please check the total population input");
		}else{
			total_population_V = std::stoi(line);
		}
		
		getline(inputReader_V, line);
		line = line.substr(line.find("=")+1);
		if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
			assert(false && "Please check the initial infective input");
		}else{
			initial_infective_V = std::stoi(line);
		}
		
		getline(inputReader_V, line);
		line = line.substr(line.find("=")+1);
		if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
			assert(false && "Please check the transmission rate input");
		}else{
			transmission_rate_V = std::stof(line);
		}
		
		cout<<mortality_V<<","<<infectivity_period_V<<","<<dt_V<<","<<incubation_period_V<<","<<total_population_V<<","<<initial_infective_V<<","<<transmission_rate_V<<endl;

        inputReader_V.close();
	}

    //default filename if none is provided
    inputReader_R.open("input_data/input_rockcliffe.txt");

    //reading input from file
    if(inputReader_R.is_open()){
        std::string line; //store read in line

        //these blocks read the input for each variable and ensure that they have been provided
        getline(inputReader_R, line);
        line = line.substr(line.find("=")+1);
        if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){ //the comparison to 13 here is to the ascii value of the carriage return character
            assert(false && "Please check the mortality input");
            //abort();
        }else{
            //cout<<"this is the line: "<<line<<endl;
            //cout<<"got here"<<endl;
            mortality_R = std::stof(line);
            //cout<<"got here"<<endl;
        }

        getline(inputReader_R, line);
        line = line.substr(line.find("=")+1);
        if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
            assert(false && "Please check the infectivity period input");
        }else{
            infectivity_period_R = std::stof(line);
        }

        getline(inputReader_R, line);
        line = line.substr(line.find("=")+1);
        if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
            assert(false && "Please check the dt input");
        }else{
            dt_R = std::stof(line);
        }

        getline(inputReader_R, line);
        line = line.substr(line.find("=")+1);
        if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
            assert(false && "Please check the incubation period input");
        }else{
            incubation_period_R = std::stof(line);
        }

        getline(inputReader_R, line);
        line = line.substr(line.find("=")+1);
        if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
            assert(false && "Please check the total population input");
        }else{
            total_population_R = std::stoi(line);
        }

        getline(inputReader_R, line);
        line = line.substr(line.find("=")+1);
        if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
            assert(false && "Please check the initial infective input");
        }else{
            initial_infective_R = std::stoi(line);
        }

        getline(inputReader_R, line);
        line = line.substr(line.find("=")+1);
        if(line.empty() || line[1]==' '|| int(line[1])==13 || int(line[0])==13){
            assert(false && "Please check the transmission rate input");
        }else{
            transmission_rate_R = std::stof(line);
        }

        cout<<mortality_R<<","<<infectivity_period_R<<","<<dt_R<<","<<incubation_period_R<<","<<total_population_R<<","<<initial_infective_R<<","<<transmission_rate_R<<endl;

        inputReader_R.close();
    }



    /****** recovered atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> pop_recovered_V = dynamic::translate::make_dynamic_atomic_model<accumulator, TIME>("recovered");
    shared_ptr<dynamic::modeling::model> pop_recovered_R = dynamic::translate::make_dynamic_atomic_model<accumulator, TIME>("recovered");

    /****** deceased atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> pop_deceased_V = dynamic::translate::make_dynamic_atomic_model<accumulator, TIME>("deceased");
    shared_ptr<dynamic::modeling::model> pop_deceased_R = dynamic::translate::make_dynamic_atomic_model<accumulator, TIME>("deceased");

    /****** infective atomic models instantiation *******************/
    shared_ptr<dynamic::modeling::model> pop_infective_V = dynamic::translate::make_dynamic_atomic_model<infective, TIME, int, float, float, float>("infective",
                                                           move(initial_infective_V), move(mortality_V), move(infectivity_period_V), move(dt_V));
    shared_ptr<dynamic::modeling::model> pop_infective_R = dynamic::translate::make_dynamic_atomic_model<infective, TIME, int, float, float, float>("infective",
                                                           move(initial_infective_R), move(mortality_R), move(infectivity_period_R), move(dt_R));
    /****** exposed atomic models instantiation *******************/
    shared_ptr<dynamic::modeling::model> pop_exposed_V = dynamic::translate::make_dynamic_atomic_model<exposed, TIME, float, float>("exposed",
                                                            move(incubation_period_V), move(dt_V));
    shared_ptr<dynamic::modeling::model> pop_exposed_R = dynamic::translate::make_dynamic_atomic_model<exposed, TIME, float, float>("exposed",
                                                            move(incubation_period_R), move(dt_R));
    /****** susceptible atomic models instantiation *******************/
    shared_ptr<dynamic::modeling::model> pop_susceptible_V = dynamic::translate::make_dynamic_atomic_model<susceptible, TIME, int, float, int, float>("susceptible",
                                                            move(total_population_V), move(transmission_rate_V), move(initial_infective_V), move(dt_V));
    shared_ptr<dynamic::modeling::model> pop_susceptible_R = dynamic::translate::make_dynamic_atomic_model<susceptible, TIME, int, float, int, float>("susceptible",
                                                            move(total_population_R), move(transmission_rate_R), move(initial_infective_R), move(dt_R));

    /*******VANIER COUPLED MODEL********/
    dynamic::modeling::Ports iports_VANIER = {typeid(inp_1)};
    dynamic::modeling::Ports oports_VANIER = {typeid(outp_1)};
    dynamic::modeling::Models submodels_VANIER = {pop_susceptible_V, pop_exposed_V, pop_infective_V, pop_recovered_V, pop_deceased_V};
    dynamic::modeling::EICs eics_VANIER = {
            dynamic::translate::make_EIC<inp_1, infective_defs::in_infective>("infective")
    };
    dynamic::modeling::EOCs eocs_VANIER = {
            dynamic::translate::make_EOC<infective_defs::out_infective, outp_1>("infective")
    };
    dynamic::modeling::ICs ics_VANIER = {
            dynamic::translate::make_IC<susceptible_defs::new_exposed, exposed_defs::in>("susceptible","exposed"),
            dynamic::translate::make_IC<exposed_defs::new_infective, infective_defs::in>("exposed","infective"),
            dynamic::translate::make_IC<infective_defs::deceased, accumulator_defs::in>("infective","deceased"),
            dynamic::translate::make_IC<infective_defs::recovered, accumulator_defs::in>("infective","recovered"),
            dynamic::translate::make_IC<infective_defs::total_infective, susceptible_defs::in>("infective","susceptible"),
            dynamic::translate::make_IC<infective_defs::out_infective, infective_defs::in_infective>("infective","infective")

    };
    shared_ptr<dynamic::modeling::coupled<TIME>> VANIER;
    VANIER = make_shared<dynamic::modeling::coupled<TIME>>(
            "VANIER", submodels_VANIER, iports_VANIER, oports_VANIER, eics_VANIER, eocs_VANIER, ics_VANIER
    );


    /*******ROCKCLIFFE COUPLED MODEL********/
    dynamic::modeling::Ports iports_ROCKCLIFFE = {typeid(inp_2)};
    dynamic::modeling::Ports oports_ROCKCLIFFE = {typeid(outp_2)};
    dynamic::modeling::Models submodels_ROCKCLIFFE = {pop_susceptible_R, pop_exposed_R, pop_infective_R, pop_recovered_R, pop_deceased_R};
    dynamic::modeling::EICs eics_ROCKCLIFFE = {
            dynamic::translate::make_EIC<inp_2, infective_defs::in_infective>("infective")
    };
    dynamic::modeling::EOCs eocs_ROCKCLIFFE = {
            dynamic::translate::make_EOC<infective_defs::out_infective, outp_2>("infective")
    };
    dynamic::modeling::ICs ics_ROCKCLIFFE = {
            dynamic::translate::make_IC<susceptible_defs::new_exposed, exposed_defs::in>("susceptible","exposed"),
            dynamic::translate::make_IC<exposed_defs::new_infective, infective_defs::in>("exposed","infective"),
            dynamic::translate::make_IC<infective_defs::deceased, accumulator_defs::in>("infective","deceased"),
            dynamic::translate::make_IC<infective_defs::recovered, accumulator_defs::in>("infective","recovered"),
            dynamic::translate::make_IC<infective_defs::total_infective, susceptible_defs::in>("infective","susceptible"),
            dynamic::translate::make_IC<infective_defs::out_infective, infective_defs::in_infective>("infective","infective")
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> ROCKCLIFFE;
    ROCKCLIFFE = make_shared<dynamic::modeling::coupled<TIME>>(
            "ROCKCLIFFE", submodels_ROCKCLIFFE, iports_ROCKCLIFFE, oports_ROCKCLIFFE, eics_ROCKCLIFFE, eocs_ROCKCLIFFE, ics_ROCKCLIFFE
    );


    /*******SEIRD COUPLED MODEL********/
    dynamic::modeling::Ports iports_SEIRD = { };
    dynamic::modeling::Ports oports_SEIRD = { };
    dynamic::modeling::Models submodels_SEIRD = {ROCKCLIFFE, VANIER};
    dynamic::modeling::EICs eics_SEIRD = { };
    dynamic::modeling::EOCs eocs_SEIRD = { };
    dynamic::modeling::ICs ics_SEIRD = {
            dynamic::translate::make_IC<outp_1, inp_2>("VANIER","ROCKCLIFFE"),
            dynamic::translate::make_IC<outp_2, inp_1>("ROCKCLIFFE","VANIER")
    };

    shared_ptr<dynamic::modeling::coupled<TIME>> SEIRD ;
    SEIRD  = make_shared<dynamic::modeling::coupled<TIME>>(
            "SEIRD ", submodels_SEIRD , iports_SEIRD , oports_SEIRD , eics_SEIRD , eocs_SEIRD , ics_SEIRD
    );


    /************** Runner call ************************/ 
    dynamic::engine::runner<float, logger_top> r(SEIRD, {0});
    r.run_until(3000);
    return 0;
}