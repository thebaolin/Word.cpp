        //Baolin Chang
        //CS135
        //Project 2 Task B
        #include <iostream>
        #include <fstream>
        #include <sstream>
        using namespace std;

        string addSpaces(string s, int width);
        string HalfSpaces(string s, int width);

        //for left and right alignment
        string addSpaces(string s, int width){
            string spaces;

            for(int i = 0; i < width - s.length() - 1; i++){
                spaces += " ";
            }

            return spaces;
        }

        //for centering
        string HalfSpaces(string s, int width){
            string halfspaces;

            int length = (int)(.5 * (width - s.length()) + 0.5);
            for(int i = 0; i < length - 1; i++){
                halfspaces += " ";
            }

            return halfspaces;
        }

        int main()
        {
            string string_width, string_outfile, words; 
            string header_align, body_align;
            bool no_alignment = false;
            string filename;
            cout << "Enter the Input Filename: ";
            cin >> filename;

            ifstream infile;
            ifstream infile2;
            infile2.open(filename);
            infile.open(filename);
            getline(infile, string_width, ';');
            int width = stoi(string_width);

            getline(infile, body_align, ';');
            if(body_align.length() == 0){
                body_align = "left";
            }

            getline(infile, header_align, ';');
                if(header_align.length() == 0)
                    header_align = body_align;
            

            string string_to_fill, string_double_spaced;
            bool to_fill, double_spaced;
            getline(infile,string_to_fill,';');
                if(string_to_fill == "true")
                    to_fill = true;
                else
                    to_fill = false;
            getline(infile, string_double_spaced, ';');
                if(string_double_spaced == "true")
                    double_spaced = true;
                else
                    double_spaced = false;

            getline(infile, string_outfile, ';');
            ofstream outfile;
            outfile.open(string_outfile);
            string firstLine;
            getline(infile, firstLine);

            string line, newstring;
            string paragraph;
            string words_list;
            int temp_width = width;
            bool seenValidLine = false;
            bool endLine = false;
            int lineCount = 0;
            bool isTitle = false;
            string dummyLine;
            int inputLineCount = 0;
            while(getline(infile2,dummyLine)){
                inputLineCount++;
            }
            if(dummyLine.length() == 0)
                inputLineCount++;

            while (getline(infile, line)){

                if(isupper(line[0]) && isupper(line[1])){
                    isTitle = true;
                    if(isTitle){
                        isTitle = false;
                        if(header_align == "right")
                            newstring += addSpaces(line,width) + line + '\n';
                        else if(header_align == "left")
                            newstring += line + addSpaces(line,width) + '\n';
                        else if(header_align == "center")
                            newstring += HalfSpaces(line,width) + line + HalfSpaces(line,width) + '\n';
                    }
                    continue;
                }

                if(line.length() == 0){
                    //add remaining words_list string before breaking new line
                    if (words_list.length() != 0){ 

                        if(body_align == "left")
                            newstring += words_list + addSpaces(words_list, width) + '\n';
                        else if(body_align == "right")
                            newstring += addSpaces(words_list, width) + words_list + '\n';
                        else   
                            newstring += HalfSpaces(words_list, width) + words_list + HalfSpaces(words_list, width) + '\n';
                    
                    //reset words_list
                    words_list = "";
                    temp_width = width;
                    }

                    newstring += '\n';

                }
                // line is shorter than width, so print it out
                else if(words_list.length() + line.length() + 1 < width ){
                        if(body_align == "left")
                            newstring += words_list+ line + addSpaces(words_list + line, width) + '\n';
                        else if(body_align == "right")
                            newstring += addSpaces(words_list + line, width) + words_list + line + '\n';
                        else   
                            newstring += HalfSpaces(words_list + line, width) + words_list + line + HalfSpaces(words_list + line, width) + '\n';

                    words_list = "";
                    temp_width = width;
                }
                else if(words_list.length() + line.length() + 1 >= width){
                    stringstream ss(line);
                    string word;

                    while(ss >> word){
                        // cout << temp_width << " " << word.length() + 1 << endl;
                        int difference = temp_width - (word.length() + 1); 
                        
                        // cout << "difference: " << difference << endl;
                        if(difference >= 0){
                            // cout<<"words list: "<<words_list<<endl;
                            if(words_list.empty()){
                                words_list = word;
                            }
                            else
                                words_list += " " + word ;
                            // cout << words_list << endl;
                            // cout<<"words list after: "<<words_list<<endl;
                            temp_width = temp_width - (word.length() + 1);
                        }
                        else{

                            if(body_align == "left")
                                newstring += words_list + addSpaces(words_list, width) + '\n';
                            else if(body_align == "right")
                                newstring += addSpaces(words_list, width) + words_list + '\n';
                            else
                                newstring += HalfSpaces(words_list, width) + words_list + HalfSpaces(words_list, width) + '\n';
                            //newstring = newstring.substr(0,newstring.length() - 1);
                            temp_width = width - (word.length() + 1);
                            words_list = word;

                        }
                    }
                }

            }

            if(words_list.length() > 0){
                if(body_align == "left")
                newstring += words_list + addSpaces(words_list, width);
                if(body_align == "right")
                    newstring += addSpaces(words_list, width) + words_list;
                if(body_align ==  "center"){
                    newstring += HalfSpaces(words_list, width) + words_list + HalfSpaces(words_list, width);
                }
            }
            

            //remove all esc chars at end of newstring
            while(newstring[newstring.length()-1] == '\n'){
            newstring = newstring.substr(0, newstring.length()-1);
            }
            for(int i = 0; i < newstring.length()-1 && i % width == 0; i++){
                if(newstring[i] == ' ' && newstring[i-1] == ' '){
                    newstring = newstring.substr(0,i) + newstring.substr(i+1,newstring.length());
                }
            }
            stringstream stream(newstring);
            string stringline, newnewstring;
            while(getline(stream,stringline)){
                 newnewstring += stringline.substr(0,width - 1) + '\n';
            }

            newnewstring = newnewstring.substr(0,newnewstring.length() - 1);    

            
            outfile<<newnewstring;
            cout<<newnewstring;
       
        } 