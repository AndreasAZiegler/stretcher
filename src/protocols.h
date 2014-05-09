#ifndef PROTOCOLS_H
#define PROTOCOLS_H

// Includes
#include <wx/listbox.h>
#include "../gui/myframe.h"
#include "experiments/experiment.h"

class Protocols
{
  public:
    Protocols(wxListBox *listbox,
              MyFrame *myframe,
              mpFXYVector *vector,
              std::string path);

    void loadProtocol(void);

    void saveProtocol(void);

    /**
     * @brief Create the preview vector and display it in the graph.
     */
    void makePreview(void);

    void runProtocol(void);

    /**
     * @brief Moves the experiment from the current position to the one before.
     * @param experimentPosition The current experiment position.
     */
    void moveExperimentUp(int experimentPosition);

    /**
     * @brief Moves the experiment from the current position to the one after.
     * @param experimentPosition The current experiment position.
     */
    void moveExperimentDown(int experimentPosition);

    /**
     * @brief Removes the Experiment at the desired position.
     * @param experimentPosition Position of the experiment.
     */
    void removeExperimentDown(int experimentPosition){
      delete m_Experiments[experimentPosition];
      m_Experiments.erase(m_Experiments.begin() + experimentPosition);
      delete m_ExperimentValues[experimentPosition];
      m_ExperimentValues.erase(m_ExperimentValues.begin() + experimentPosition);
    }

    /**
     * @brief Adds an experiment.
     * @param experiment Pointer to the experiment object.
     */
    void addExperiment(Experiment *experiment){
      m_Experiments.push_back(experiment);
      m_ExperimentValues.push_back(m_Experiments.back()->getExperimentValues());
    }

    /**
     * @brief Sets the experiment start time point.
     */
    void setStartPoint(void);

    /**
     * @brief Exports the measurement data to a .csv file.
     */
    void exportCSV(std::vector<bool> *disableexport);

    /**
     * @brief Returns the current experiment type as a string.
     * @return The current experiment type as a string.
     */
    std::string experimentTypeToString();

  private:
    MyFrame *m_MyFrame;
    mpFXYVector *m_Vector;
    wxListBox *m_ListBox;
    std::vector<Experiment*> m_Experiments;
    std::vector<ExperimentValues*> m_ExperimentValues;
    std::string m_StoragePath;																							/**< Storage path as a std::string */
    std::chrono::high_resolution_clock::time_point m_StartTimePoint;				/**< Start time point of the experiment. */
};

#endif // PROTOCOLS_H
