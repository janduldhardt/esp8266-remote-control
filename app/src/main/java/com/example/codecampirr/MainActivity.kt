package com.example.codecampirr

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.google.firebase.database.FirebaseDatabase
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

    private var isOn = false
    private var currentTemp = 20
    val path = "airConference"
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        setTextViewsInvisible()

        btn_temp_up.setOnClickListener {
            powerTempUpAction()
        }

        btn_temp_down.setOnClickListener {
            powerTempDownAction()
        }


        btn_power.setOnClickListener {
            powerOnButtonAction()
        }

        btn_send.setOnClickListener {
            sendButtonAction()
        }
    }

    private fun sendButtonAction() {
        if (!isOn) {
            sendStatusToFirebase("off")
        } else {
            sendStatusToFirebase()
        }
    }

    private fun powerTempUpAction() {
        if (!isOn || currentTemp == 27) {
            return
        }
        currentTemp++
        tv_temp.text = currentTemp.toString()
        sendStatusToFirebase()
    }

    private fun powerTempDownAction() {
        if (!isOn || currentTemp == 18) {
            return
        }
        currentTemp--
        tv_temp.text = currentTemp.toString()

        sendStatusToFirebase()
    }


    private fun powerOnButtonAction() {
        isOn = !isOn

        if (isOn) {
            sendStatusToFirebase()
            tv_temp.text = currentTemp.toString()
            tv_celcius.text = "C°"
        } else {
            sendStatusToFirebase("off")
            setTextViewsInvisible()
            setTextCelsiusInvisible()
        }
    }

    private fun sendStatusToFirebase(status: String = currentTemp.toString()) {
        val database = FirebaseDatabase.getInstance()
        val myRef = database.getReference("airConference/status")

        myRef.setValue(status)
    }


    private fun setTextViewsInvisible() {
        tv_temp.text = ""
    }

    private fun setTextCelsiusInvisible() {
        tv_celcius.text = ""
    }

}
