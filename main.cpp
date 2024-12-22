#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_File_Chooser.H>
#include <iostream>
#include <string>
#include <vector>
#include <sndfile.h>

#define BUFFER_LEN 4096


std::string selectedFilePath;
float volumeFactor = 1.0f;

void adjust_volume(float *buffer, sf_count_t count, float volume_scale) {
    for (sf_count_t i = 0; i < count; i++) {
        buffer[i] *= volume_scale;
    }
}

std::string getFileName(const char* filePath) {
    std::string path(filePath);
    size_t pos = path.find_last_of("/\\");
    return (pos == std::string::npos) ? path : path.substr(pos + 1);
}

void changeVolume(const char* inputFile, const char* outputFile, float volumeFactor) {

    SF_INFO audioFileInfo;
    SNDFILE *audioFile, *outfile;
    float buffer[BUFFER_LEN];
    sf_count_t read_count;

    if (!(audioFile = sf_open(inputFile, SFM_READ, &audioFileInfo))) {
        printf("Error: Could not open input file '%s'\n", inputFile);
        
    }
    if (!(outfile = sf_open(outputFile, SFM_WRITE, &audioFileInfo))) {
        printf("Error: Could not open output file '%s'\n", outputFile);
        sf_close(outfile);
    }
    while ((read_count = sf_read_float(audioFile, buffer, BUFFER_LEN)) > 0) {
        adjust_volume(buffer, read_count, volumeFactor);
        sf_write_float(outfile, buffer, read_count);
    }
    
    sf_close(outfile);
    sf_close(audioFile);
}


void selectFile(Fl_Widget* widget, void* data) {
    Fl_Input* inputFile = static_cast<Fl_Input*>(data);

    
    const char* file = fl_file_chooser("Выберите WAV файл", "*.wav", nullptr);
    if (file) {
        selectedFilePath = file;                  
        inputFile->value(getFileName(file).c_str()); 
    }
}


void on_volume_change(Fl_Widget* widget, void*) {
    Fl_Slider* slider = static_cast<Fl_Slider*>(widget);
    volumeFactor = slider->value(); 
}


void on_apply(Fl_Widget*, void*) {
    if (selectedFilePath.empty()) {
        std::cerr << "Файл не выбран!" << std::endl;
        return;
    }

    std::string outputFilePath = selectedFilePath.substr(0, selectedFilePath.find_last_of('.')) + "_modified.wav";
    

    changeVolume(selectedFilePath.c_str(), outputFilePath.c_str(), volumeFactor);
}

int main() {
    Fl_Window* window = new Fl_Window(400, 300, "Audio Volume Editor");

    
    Fl_Input* inputFile = new Fl_Input(100, 30, 200, 30, "Файл (.wav):");
    inputFile->readonly();

    
    Fl_Button* browseButton = new Fl_Button(310, 30, 70, 30, "Обзор");
    browseButton->callback(selectFile, inputFile); 

   
    Fl_Slider* volumeSlider = new Fl_Slider(100, 130, 200, 30, "Громкость");
    volumeSlider->type(FL_HORIZONTAL);
    volumeSlider->bounds(0.1, 2.0); 
    volumeSlider->value(1.0); 
    volumeSlider->callback(on_volume_change);

    
    Fl_Button* applyButton = new Fl_Button(150, 200, 100, 30, "Применить");
    applyButton->callback(on_apply);

    window->end();
    window->show();

    return Fl::run();
}
