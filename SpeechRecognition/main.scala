import edu.cmu.sphinx.frontend.util.Microphone;
import edu.cmu.sphinx.recognizer.Recognizer;
import edu.cmu.sphinx.result.Result;
import edu.cmu.sphinx.util.props.ConfigurationManager;

object main{
  def main(args : Array[String]){
    var cm : ConfigurationManager = new ConfigurationManager(this.getClass.getResource("/helloworld.config.xml"))
    var recognizer : Recognizer = cm.lookup("recognizer").asInstanceOf[Recognizer];
    recognizer.allocate();
    var mic : Microphone = cm.lookup("microphone").asInstanceOf[Microphone];
    if (!mic.startRecording()){
      System.out.println("Can't get mic\n");
      recognizer.deallocate();
    }
    println("Initialized mic");
    while(true){
      println("Entered loop")
      var result : Result = recognizer.recognize();
      if (result != null){
        println(result.getBestFinalResultNoFiller());
      }
      else{
        println("Couldn't hear that...")
      }
      mic.clear()
    }

  }
}
