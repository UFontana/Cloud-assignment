import React, { useState, useEffect } from 'react'
import {
  BrowserRouter as Router,
  Routes,
  Route,
  Navigate
} from "react-router-dom";
import axios from "axios";
import Login from './Login'
import User from './User';
import AllUsers from './AllUsers';
import Cookies from 'universal-cookie';

const SERVER_IP = process.env.REACT_APP_SERVER_IP;
const PORT = process.env.REACT_APP_PORT;
const ADDRESS = `${SERVER_IP}:${PORT}`;

function App() {
  const cookies = new Cookies();
  const [list, setList] = useState([]);
  const [redirect, setRedirect] = useState(null);
  const [posts, setPosts] = useState([]);

  useEffect(() => {
    getAllUsers();
  }, [posts]);
  

  const getAllUsers = async () => {
    let config = {
      headers: { "Access-Control-Allow-Origin": "*" },
    };

    axios.get(
      `http://${ADDRESS}/getAllUsers`,
      config
    )
    .then((res) => {
      setList(res.data.users);
      console.log(res);
    })
    .catch((err) => {
      console.log(err);
    });
  }

  const addUser = async (id, name) => {
    let config = {
      headers: { "Access-Control-Allow-Origin": "*" },
    };
    let userInfo = {
      userId: id, 
      userName: name
    };

    axios.post(
      `http://${ADDRESS}/addUser`, userInfo, config
    )
    .then((res) => {
      console.log(res);
      cookies.set("userName", name, { path: '/' })
      cookies.set("userId", id, { path: '/' })
      getAllUsers()
      setTimeout(function(){
        setRedirect("/users")
      }, 1000);
    })
    .catch((err) => {
      console.log(err);
    });
  }


  const allUserPosts = async (id) => {
    let config = {
      headers: { "Access-Control-Allow-Origin": "*" },
    };

    axios.get(
      `http://${ADDRESS}/allUserPosts`,
      { params: { userId: id } },
      config
    )
    .then((res) => {
      if (typeof res.data.posts === 'undefined') {
        setPosts([]);
      } else {
        console.log(res);
        setPosts(res.data.posts);
      }      
    })
    .catch((err) => {
      console.log(err);
      setPosts([]);
    });
  }

  const addPost = async (id, content, userId) => {
    if (!id.match(/^post_[0-9]+$/)) {
      alert("Post ID must be in the following format: post_N")
    } else {
      let config = {
        headers: {"Access-Control-Allow-Origin": "*"},
      };
      let postInfo = {
        postId: id,
        postContent: content,
        userId: userId,
      };

      axios.post(
          `http://${ADDRESS}/addPost`,
          postInfo,
          config)
          .then((res) => {
            console.log(res);
          })
          .catch((err) => {
            console.log(err);
          });
    }
  };

  const deleteUser = async (id) => {
    let config = {
      headers: { "Access-Control-Allow-Origin": "*" },
    };
    let delInfo = {
      id: id,
    };

    let res = await axios.post(
      `http://${ADDRESS}/deleteUser`,
      delInfo,
      config
    )
    
    console.log(res);
  }

  const deletePost = async (post_id, user_id) => {
    let config = {
      headers: { "Access-Control-Allow-Origin": "*" },
    };
    let delInfo = {
      postId: post_id,
      userId: user_id
    };

    let res = await axios.post(
      `http://${ADDRESS}/deletePost`,
      delInfo,
      config
    )
    
    console.log(res);
  }

  function signIn(userId) {
    getAllUsers()
    const isFound = list.some(user => user.userId === userId)
    if (!isFound) {
      alert("User ID not found");
    } else {      
      let foundObj = list.find(user => user.userId === userId)
      cookies.set("userName", foundObj.userName, { path: '/' })
      cookies.set("userId", foundObj.userId, { path: '/' })
      setRedirect("/users")   
    }
  }

  function signUp(userName, userId) {
    if (!userId.match(/^user_[0-9]+$/)) {
      alert("User ID must be in the following format: user_N")
    } else {
      let userIdDupl = list.some(user => user.userId === userId)
      let userNameDupl = list.some(user => user.userName === userName)
      if (userIdDupl && userNameDupl) {
        alert("please choose with another id and name")
      } else if (userIdDupl) {
        alert("please choose another id")
      } else if (userNameDupl) {
        alert("please choose another username")
      } else {
        addUser(userId, userName)
      }
    }
  }

  function logout() {
      cookies.remove("userName")
      cookies.remove("userId")
      setRedirect("")
      setTimeout(function(){
        if (!(cookies.get("userId"))) {
          return <Navigate to="/" />
        }
      }, 2000);
  }

  function updatePosts(userId) {
    setTimeout(() => { allUserPosts(userId); }, 500);
  }

  function deletePostAndUpdate(userId, postId) {
    deletePost(postId, userId)
    updatePosts(userId)
  }

  function post(postId, postContent, userId) {
    addPost(postId, postContent, userId)
    updatePosts(userId)
  }

  return (
    <div className="container">
      <Router>
        <Routes>
          <Route exact path="/" element={
            <Login 
              redirect={redirect}
              signIn={signIn} 
              signUp={signUp}
              />
            }></Route>
          <Route exact path="/users" element={<AllUsers list={list}  
                                                        logout={logout} 
                                                        userName={cookies.get("userName")}/>}></Route>
          <Route exact path="/user/:userId/:userName" element={<User signedInUserId={cookies.get("userId")}
                                                              updatePosts={updatePosts}
                                                              deletePostAndUpdate={deletePostAndUpdate}
                                                              deleteUser={deleteUser}
                                                              logout={logout}
                                                              posts={posts}
                                                              post={post}></User>}></Route>
          <Route element={<Error></Error>}></Route>
        </Routes>
      </Router>
    </div>
  );
}

function Error() {
  return <div>link not found</div>;
}

export default App;