import React from 'react';
import ReactDOM from 'react-dom';
import {Router, Route, hashHistory, IndexRoute,} from 'react-router';

import HomeContainer from './src/components/home'
import PayContainer from './src/components/pay'
import ChargeContainer from './src/components/charge'
import OrderPayContainer from './src/components/order'

import {Session_Storage_Account, sessionSaveOneData} from "./src/util/sessionStorage";

import './index.less';

class App extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            title: 'app',
            open: false,
        };
    }

    render() {
        // console.log(this.props.route, this.props.params, this.props.routeParams);
        return (
            <div className='container'>
                {
                    this.props.children
                }
            </div>
        );
    }
}




ReactDOM.render(
    <Router history={hashHistory}>
        <Route path="/" component={App}>
            <IndexRoute component={HomeContainer}/>
            <Route path="/pay" component={PayContainer}/>
            <Route path="/charge" component={ChargeContainer}/>
            <Route path="/order" component={OrderPayContainer}/>

            <Route path="/p" component={()=>{
                sessionSaveOneData(Session_Storage_Account,'alice');
                hashHistory.push('/');
                return null
            }}/>
            <Route path="/s" component={()=>{
                sessionSaveOneData(Session_Storage_Account,'bob');
                hashHistory.push('/');
                return null
            }}/>
        </Route>
    </Router>
    , document.getElementById('app-container'));
