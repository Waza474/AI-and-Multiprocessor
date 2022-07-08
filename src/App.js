import { BrowserRouter as Router, Route, Switch } from 'react-router-dom';
import Home from './Pages/Home';
import Lecture from './Pages/Lecture';
import Classroom from './Pages/Classroom';
import Login from './Pages/Login';
import CreateMeeting from './Components/Dashboard/CreateMeeting';
import JoinMeeting from './Components/Dashboard/JoinMeeting';
function App() {
  return (
    <Router>
      <Switch>
        <Route exact path="/">
          <Home />
        </Route>
        <Route path="/lecture">
          <Lecture />
        </Route>
        <Route path="/classroom">
          <Classroom />
        </Route>
        <Route path="/login">
          <Login/>
        </Route>
        <Route path ="/createmeeting" component={CreateMeeting}/>
        <Route path ="/joinmeeting" component={JoinMeeting}/>
      </Switch>
    </Router>
  );
}

export default App;
